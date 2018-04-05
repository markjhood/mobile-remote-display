/*
 * This program provides the remote display component of the MWC 2010 MIDP3
 * remote Auxiliary Display demo.
 */
#include <unistd.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <signal.h>

#include "cmd.h"
#include "pixbufs.h"

#define SMALL_DASH_BACKGROUND "car-headunit-640x480.jpg"
#define LARGE_DASH_BACKGROUND "car-headunit-1024x768.jpg"
#define SLIDE_BACKGROUND      "slide-800x600.jpg"

static void 
usage()
{
    fprintf(stderr,
"usage: remote [-port <n>] [-connect <ipaddr>] [-s <style>] [-bg <0|1>]\n");
    fprintf(stderr, "  The port should be a number between 1023 and 65536.\n");
    fprintf(stderr, "If no port is supplied then %d is used.\n", DEFAULT_PORT);
    fprintf(stderr, "  The default is to listen for an incoming connection\n");
    fprintf(stderr, "on the specified port.  If an IP address is supplied\n");
    fprintf(stderr, "with the '-connect' option then a connection will be\n");
    fprintf(stderr, "made to the specfied host instead.\n");
    fprintf(stderr, "  If the '-s' option is used it must be followed by\n");
    fprintf(stderr, "either 1, to specify the 640x480 dashboard mockup; 2,\n");
    fprintf(stderr, "to specify the 1024x768 dashboard mockup; or 3, to\n");
    fprintf(stderr, "specify the 800x600 unadorned presentation window.\n");
    fprintf(stderr, "The default style is 1, the 640x480 dashboard mockup.\n");
    fprintf(stderr, "  If '-bg' is set to 1 then the program runs in the\n");
    fprintf(stderr, "background, otherwise it remains in the foreground.\n");
    fprintf(stderr, "  Options can be abbreviated.  If only one argument is\n");
    fprintf(stderr, "provided then it is taken as the port number.\n");
    exit(0);
}

static fb f;
static stream s;

static int port;
static int socket;
static int verbose;
static int inputTag;
static int demoMode;
static char *ipaddr;

static void onTerm(int data);
static gint onClose(GtkWidget *widget, GdkEvent *event, gpointer data);
static gint onExpose(GtkWidget *widget, GdkEvent *event, gpointer data);
static void onRead(gpointer widget, gint socket, GdkInputCondition condition);

static gpointer waitForConnection(gpointer data);
static GdkPixbuf *bgPixbuf = NULL;

#ifdef MAC_INTEGRATION
#include "ige-mac-menu.h"
#include "ige-mac-dock.h"
#include "ige-mac-bundle.h"

static void
onDockClick(IgeMacDock *dock, GtkWidget *window)
{
    if (gtk_widget_get_visible(window)) {
	gtk_widget_hide(window);
    } else {
	gtk_widget_show(window);
    }
}
#endif

int 
main(int argc, char *argv[])
{
    int pid, bg;
    char *bgImageFile = NULL;

    GError *error = NULL;
    GtkWidget *win, *menubar, *quit_item, *vbox;

#ifdef MAC_INTEGRATION
    IgeMacDock *dock;
#endif

    bg = 0;
    ipaddr = 0;
    socket = 0;
    verbose = 1;
    port = DEFAULT_PORT;
    demoMode = FB_SMALL_DASH;

    while (--argc > 0 && (*++argv)[0] == '-') {
	if (--argc < 1) {
	    usage(); /* options must be followed by values */
	}
	switch (*++(argv[0])) {
	    case 'p':
	    case 'P':
		port = atoi(*++argv);
		break;
	    case 'c':
	    case 'C':
		ipaddr = *++argv;
		break;
	    case 's':
	    case 'S':
		demoMode = atoi(*++argv);
		break;
	    case 'b':
		bg = atoi(*++argv);
		break;
	    default:
		usage();
	}
    }
    switch (argc) {
	case 1:
	    port = atoi(*argv); /* interpret single arg as port number */
	case 0:
	    break;
	default:
	    usage();
    }
    if (port < 1024 || port > 65535) {
	usage();
    }

    if (ipaddr) {
	printf("Connecting to %s on port %d\n", ipaddr, port);
    } else {
	printf("Listening on port %d\n", port);
    }

    if (bg) {
	/*
	 * The `-bg 1' flag must be specified when the remote display is
	 * launched from blueproximity in order to prevent the launcher from
	 * blocking.  We also need to close stdin, stdout, and stderr.
	 */
	printf("Running in the background...");
	if((pid = fork()) != 0) {
	    printf(" pid %d\n", pid);
	    exit(0);
	}
	close(0); close(1); close(2);
	verbose = 0;
    } 

    /*
     * Initialize the stream and framebuffer APIs.
     */
    f = fbNew(demoMode, verbose);
    s = streamNew(fbImageSize(f) + 100, verbose);

    /*
     * Initialize GTK.
     */
    g_thread_init(NULL);
    gdk_threads_init();
    gdk_threads_enter();
    gtk_init(&argc, &argv);

    /* create the top level GTK window */
    win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size
	(GTK_WINDOW(win), fbWindowWidth(f), fbWindowHeight(f));

    /* set up title and position */
    gtk_window_set_title(GTK_WINDOW(win), "Aplix MIDP3 Remote Display");
    gtk_window_set_position(GTK_WINDOW(win), GTK_WIN_POS_CENTER_ALWAYS);
    gtk_window_set_keep_above(GTK_WINDOW(win), TRUE);

    /* set up background image if needed */
    switch (demoMode) {
	case FB_LARGE_DASH:
	    bgImageFile = find_image_file(LARGE_DASH_BACKGROUND, &error);
	    if (!bgImageFile) goto loadError;
	    break;
	case FB_SMALL_DASH:
	    bgImageFile = find_image_file(SMALL_DASH_BACKGROUND, &error);
	    if (!bgImageFile) goto loadError;
	    break;
	case FB_PRESENTATION:
	    bgImageFile = find_image_file(SLIDE_BACKGROUND, &error);
	    if (!bgImageFile) goto loadError;
	    break;
	default:
	    bgImageFile = NULL;
	    break;
    }

    if (bgImageFile) {
	bgPixbuf = gdk_pixbuf_new_from_file(bgImageFile, &error);
	if (error || !bgPixbuf) goto loadError;
	g_free(bgImageFile);
    } else {
	bgPixbuf = NULL;
    }

    /* create a menu bar with a Quit item for Mac OS X, hidden on Linux */
    menubar = gtk_menu_bar_new();
    quit_item = gtk_menu_item_new_with_label("Quit");
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), quit_item);

    vbox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(win), vbox);
    gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, TRUE, 0);
    
#ifdef MAC_INTEGRATION
    /*
     * We need some integration with the Mac OS X desktop to allow the remote
     * display to respond to Quit events from the app menu and Dock.  We also
     * map Dock clicks to a hide/unhide function for the display.
     */
    ige_mac_menu_set_menu_bar(GTK_MENU_SHELL(menubar));
    ige_mac_menu_set_quit_menu_item(GTK_MENU_ITEM(quit_item));
    g_signal_connect(quit_item, "activate", G_CALLBACK(onClose), NULL);

    dock = ige_mac_dock_new();
    g_signal_connect(dock, "quit-activate", G_CALLBACK(onClose), win);
    g_signal_connect(dock, "clicked", G_CALLBACK(onDockClick), win);
#endif

    /* event callbacks */
    g_signal_connect(quit_item, "select", G_CALLBACK(onClose), NULL);
    g_signal_connect(win, "delete-event", G_CALLBACK(onClose),  NULL);
    if (demoMode == FB_PRESENTATION) {
	/*
	 * This mode doesn't employ a customized background skin with an inset
	 * display area, but we still want to display a background image on
	 * expose if it's available.
	 */
	g_signal_connect(win, "expose-event", G_CALLBACK(onExpose), NULL);
	if (bgPixbuf) {
	    fbBgImageRGB24(f, fbDisplayX(f), fbDisplayY(f),
			   fbDisplayWidth(f), fbDisplayHeight(f),
			   gdk_pixbuf_get_pixels(bgPixbuf));
	}
    } else {
	g_signal_connect(win, "expose-event", G_CALLBACK(onExpose), bgPixbuf);
    }

    /* trap TERM so we can send a DISPLAY ABSENT event before exiting */
    signal(SIGTERM, onTerm);

    /* start a thread to wait for a connection */
    g_thread_create(waitForConnection, win, FALSE, NULL);

    /* enter the GTK event loop */
    gtk_widget_show_all(win);
    gtk_widget_hide(menubar);
    gtk_main();
    exit(0);

    loadError:
    if (bgImageFile) {
	g_free(bgImageFile);
    }
    if (verbose) {
	if (error->domain == GDK_PIXBUF_ERROR)
	    g_print("Pixbuf Related Error:\n");
	if (error->domain == G_FILE_ERROR)
	    g_print("File Error: Check file permissions and state:\n");
	g_printerr("%s\n", error[0].message);
    }
    exit(1);
}

static void 
onRead(gpointer widget, gint socket, GdkInputCondition condition)
{
    int length;
    unsigned int *cmd;
    unsigned char opcode;

    /* called outside the main GTK+ lock, so obtain the lock first */
    gdk_threads_enter();
    switch (opcode = streamReadCommand(s, &cmd, &length)) {
	case FLUSH:
	    /*
	     * Set up an expose event to repaint the display.  Give the GTK
	     * main loop cycles to process the expose by removing the onRead()
	     * listener until the expose event is processed.  This prevents us
	     * from spending all our time reading.
	     *
	     * The check for FB_IMAGE_NULL works around the behavior of some
	     * clients that drop the connection after a session, but then
	     * immediately restore the connection and start sending FLUSH
	     * commands without updating the previous connection's image.
	     */
	    if (fbImageFormat(f) != FB_IMAGE_NULL) {
		gtk_widget_show(widget);
		if (inputTag) {
		    gdk_input_remove(inputTag);
		    inputTag = 0;
		}
		gtk_widget_queue_draw_area
		    (widget, fbDisplayX(f), fbDisplayY(f),
		     fbDisplayWidth(f), fbDisplayHeight(f));
	    }
	    break;
	case STREAM_DISCONNECT:
	    /*
	     * The remote connection closed down on us.  Shut down input
	     * processing and wait for the next connection.
	     */
	    if (inputTag) {
		gdk_input_remove(inputTag);
		inputTag = 0;
	    }
	    if (streamSocket(s) != -1) {
		streamClose(s);
	    }
	    if (demoMode == FB_SMALL_DASH || demoMode == FB_LARGE_DASH) {
		/*
		 * If running a car dashboard mockup, assume the connection
		 * closed due to proximity state and hide the widget until the
		 * connection is re-established.  Set the image to NULL status
		 * so that only the background skin gets drawn on expose.
		 */
		gtk_widget_hide(widget);
		fbImageNull(f);
	    } else if (bgPixbuf) {
		/*
		 * The demo mode doesn't use a background skin with an inset
		 * display area, but we have a full-window background.
		 */
		fbBgImageRGB24(f, fbDisplayX(f), fbDisplayY(f),
			       fbDisplayWidth(f), fbDisplayHeight(f),
			       gdk_pixbuf_get_pixels(bgPixbuf));
	    } else {
		fbImageNull(f);
	    }

	    g_thread_create(waitForConnection, widget, FALSE, NULL);
	    break;
	default:
	    fbDispatch(f, opcode, cmd, length);
	    break;
    }
    gdk_threads_leave();
}

static gint
onExpose(GtkWidget *widget, GdkEvent *event, gpointer pixbuf)
{
    if (pixbuf &&
	(event->expose.area.x != fbDisplayX(f) ||
	 event->expose.area.y != fbDisplayY(f) ||
	 event->expose.area.width  != fbDisplayWidth(f) ||
	 event->expose.area.height != fbDisplayHeight(f))) {
	/*
	 * Got a real expose event instead of a response to
	 * gtk_widget_queue_draw_area(), so redraw the background skin.
	 * 
	 * Originally we rendered the skin into the window background so
	 * we didn't have to process this expose explicitly:
	 *
	 * GdkPixmap *background;
	 * GtkStyle *style;
	 * gdk_pixbuf_render_pixmap_and_mask(pixbuf, &background, NULL, 0);
	 * style = gtk_widget_get_style(window);
	 * style->bg_pixmap[0] = background;
	 * gtk_widget_set_style(GTK_WIDGET(window), GTK_STYLE(style));
	 *
	 * Unfortunately that currently doesn't work with Mac GTK-OSX.
	 */
        int w = gdk_pixbuf_get_width((GdkPixbuf *)pixbuf);
        int h = gdk_pixbuf_get_height((GdkPixbuf *)pixbuf);

	w = w < event->expose.area.width  ? w : event->expose.area.width;
	h = h < event->expose.area.height ? h : event->expose.area.height;

	gdk_draw_pixbuf(widget->window, widget->style->fg_gc[GTK_STATE_NORMAL],
			(GdkPixbuf *)pixbuf,
			event->expose.area.x, event->expose.area.y,
			event->expose.area.x, event->expose.area.y, w, h,
			GDK_RGB_DITHER_NORMAL, 0, 0);
    }

    /*
     * Update the received image if any.  The image format is determined by
     * the last IMAGE command and must be consistent within each frame.
     */
    switch (fbImageFormat(f)) {
	case FB_IMAGE_NULL:
	    break;
	case FB_IMAGE_RGB32:
	    gdk_draw_rgb_32_image(widget->window,
				  widget->style->fg_gc[GTK_STATE_NORMAL],
				  fbDisplayX(f), fbDisplayY(f),
				  fbDisplayWidth(f), fbDisplayHeight(f),
				  GDK_RGB_DITHER_NORMAL,
				  (unsigned char *)fbImage(f),
				  fbDisplayWidth(f) * 4);
	    break;
	case FB_IMAGE_RGB24:
	    gdk_draw_rgb_image(widget->window,
			       widget->style->fg_gc[GTK_STATE_NORMAL],
			       fbDisplayX(f), fbDisplayY(f),
			       fbDisplayWidth(f), fbDisplayHeight(f),
			       GDK_RGB_DITHER_NORMAL,
			       (unsigned char *)fbImage(f),
			       fbDisplayWidth(f) * 3);
	    break;
	case FB_IMAGE_RGB565:
	    if (verbose)
		fprintf(stderr, "FB_IMAGE_RGB565 unimplemented.\n");
	    break;
	default:
	    if (verbose)
		fprintf(stderr, "unknown image format %d\n", fbImageFormat(f));
	    break;
    }
    
    /*
     * Restore input listener if disabled and save the returned tag.  Initial
     * expose event usually occurs before a connection, so check the socket.
     */
    if (streamSocket(s) != -1 && inputTag == 0) {
	inputTag = gdk_input_add(streamSocket(s),
				 GDK_INPUT_READ, onRead, widget);
    }

    return TRUE;
}

static gpointer
waitForConnection(gpointer widget)
{
    /* block this thread until we get a connection */
    streamOpen(s, ipaddr, port);
    gdk_threads_enter();

    /*
     * Add an input listener and save the returned tag.  This is run outside
     * the main GTK+ lock so we synchronize with gdk_threads_enter() and
     * gdk_threads_leave().  Originally we also had the call to
     * gtk_widget_show() here, but that provokes a nasty error from
     * NSAutoreleaseNoPool() on Mac OS X.
     *
     * We must not call gdk_input_add() more than once for a given stream
     * before calling gdk_input_remove().  The initial expose from the GTK
     * event thread may have called gdk_input_add() already in the time
     * between creating the socket, getting the lock, and reaching this point,
     * so we need to check inputTag.  We only access inputTag within the GTK
     * lock, so we're safe in the time between checking the tag and calling
     * gdk_input_add().
     */
    if (inputTag == 0) {
	inputTag = gdk_input_add(streamSocket(s),
				 GDK_INPUT_READ, onRead, widget);
    }
    gdk_threads_leave();

    /* set the remote display state */
    fbSetState(f, FB_DISPLAY_HARDWARE_ENABLED, FB_STATE_VISIBLE);

    /* send the required initial DISPLAY event to the phone */
    sendDisplayEvent(s, f, verbose);
    return NULL;
}


static gint 
onClose(GtkWidget *widget, GdkEvent *event, gpointer data)
{
    /*
     * Remove the onRead() listener since it runs asynchronously from GTK and
     * sometimes continues to read commands before the process exits; a flush
     * command can then queue a redraw on a non-existent window, provoking an
     * ugly `Gtk-CRITICAL' error message.  Also make sure inputTag is non-zero,
     * or we'll sometimes get an equally ugly `Glib-CRITICAL' error.
     */
    if (inputTag) {
	gdk_input_remove(inputTag);
	inputTag = 0;
    }
    if (streamSocket(s) != -1) {
	fbSetState(f, FB_DISPLAY_HARDWARE_ABSENT, FB_STATE_BACKGROUND);
	sendDisplayEvent(s, f, verbose);
	streamClose(s);
    }
    if (verbose) {
	fprintf(stderr, "Locally initiated quit.\n");
    }

    fbFree(f);
    streamFree(s);
    gtk_main_quit();
    exit(0);
}

static void
onTerm(int signal)
{
    /*
     * Called in response to SIGTERM.  Note that the whole process is blocked
     * now, so we can't use gdk_threads_enter() and obtain a lock that will
     * allow us to safely call GTK functions.  The only thing we'll do is exit
     * after sending the DISPLAY event.
     */
    if (verbose) {
	fprintf(stderr, "Got SIGTERM, sending hardware absent event.\n");
    }
    if (streamSocket(s) != -1) {
	fbSetState(f, FB_DISPLAY_HARDWARE_ABSENT, FB_STATE_BACKGROUND);
	sendDisplayEvent(s, f, verbose);
	streamClose(s);
    }

    fbFree(f);
    streamFree(s);
    exit(0);
}
