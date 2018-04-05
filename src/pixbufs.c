/*
 * This code originally came from gtk/demos/gtk-demo/pixbufs.c.  It has been
 * hacked for the Aplix remote display demotest program.
 */

/* Pixbufs
 *
 * A GdkPixbuf represents an image, normally in RGB or RGBA format.
 * Pixbufs are normally used to load files from disk and perform
 * image scaling.
 *
 * This demo is not all that educational, but looks cool. It was written
 * by Extreme Pixbuf Hacker Federico Mena Quintero. It also shows
 * off how to use GtkDrawingArea to do a simple animation.
 *
 * Look at the Image demo for additional pixbuf usage examples.
 *
 */

#include <stdlib.h>
#include <gtk/gtk.h>
#include <math.h>
#include <string.h>

#define FRAME_DELAY 50

static const char *image_names[] = {
    "apple-red.png",
    "gnome-applets.png",
    "gnome-calendar.png",
    "gnome-foot.png",
    "gnome-gmush.png",
    "gnome-gimp.png",
    "gnome-gsame.png",
    "gnu-keys.png"
};
#define N_IMAGES G_N_ELEMENTS (image_names)

/* Images */
static GdkPixbuf *images[N_IMAGES];

/*
 * Image directories.
 *
 * The first entry is a null pointer that is replaced by a reference to the
 * value of the runtime BUNDLE_IMGDIR environment variable.  This is set by a
 * launcher script to point to the location of the resource directory for the
 * remote-jblend.app Mac OS X bundle.
 *
 * INSTALL_IMGDIR is defined by the Makefile for Linux builds and should point
 * to a static installation directory once the executables are installed.
 *
 * The other relative directories are convenient for running the executables
 * in the development tree without installing binaries or bundles.
 */
static char *image_paths[] = {
    0,
#ifdef INSTALL_IMGDIR
    INSTALL_IMGDIR,
#endif
    "images",
    "../images",
    "../../images",
    "../../../images",
    "src/images",
    "../src/images",
    "../../src/images",
    "../../../src/images"
};
#define N_PATHS G_N_ELEMENTS (image_paths)

#define BACKGROUND_320x240 "background-320x240.jpg"
#define BACKGROUND_600x400 "background-600x400.jpg"
#define BACKGROUND_800x600 "background-800x600.jpg"

/* Current frame and previous frame for difference analysis */
static GdkPixbuf *frame1;
static GdkPixbuf *frame2;

/* Background image */
static GdkPixbuf *background;
static gint back_width, back_height;

/*
 * Look in known directories for the named file.  Always returns a string that
 * must be freed with g_free(), unless NULL is returned for error.
 */
char *
find_image_file(const char *base, GError **error)
{
    char *filename;
    int i, length = 0;

    if (!image_paths[0]) {
	/* check for Mac OS X bundle */
	image_paths[0] = getenv("BUNDLE_IMGDIR");
	if (!image_paths[0]) image_paths[0] = "";
    }

    for (i = 0; i < N_PATHS; i++) {
	filename = g_build_filename(image_paths[i], base, NULL);
	if (g_file_test(filename, G_FILE_TEST_EXISTS)){
	    return filename;
	}
	g_free(filename);
	if (image_paths[i])
	    length += strlen(image_paths[i]);
    }

    /* put expected image file directories in error string */
    filename = malloc(length + 1 + N_PATHS);
    strcpy(filename, image_paths[0]);
    for (i = 1; i < N_PATHS; i++) {
	strcat(filename, "\n");
	if (image_paths[i])
	    strcat(filename, image_paths[i]);
    }

    g_set_error(error, G_FILE_ERROR, G_FILE_ERROR_NOENT,
		"Could not find image file %s in any directories:\n%s",
		base, filename);

    free(filename);
    return NULL;
}

/* Loads the images for the demo and returns whether the operation succeeded */
static gboolean
load_pixbufs(int w, int h)
{
  gint i;
  char *filename = NULL;
  GError *error = NULL;

  switch (w) {
      case 320:
	  filename = find_image_file(BACKGROUND_320x240, &error);
	  break;
      case 600:
	  filename = find_image_file(BACKGROUND_600x400, &error);
	  break;
      case 800:
	  filename = find_image_file(BACKGROUND_800x600, &error);
	  break;
      default:
	  fprintf(stderr, "load_pixbufs(): unsupported width %d pixels.\n", w);
	  exit(1);
  }

  if (!filename)
      goto loadError;

  background = gdk_pixbuf_new_from_file(filename, &error);
  if (error || !background)
      goto loadError;

  g_free(filename);
  back_width = gdk_pixbuf_get_width(background);
  back_height = gdk_pixbuf_get_height(background);

  for (i = 0; i < N_IMAGES; i++)
    {
	filename = find_image_file(image_names[i], &error);
	if (!filename)
	    goto loadError;
	
	images[i] = gdk_pixbuf_new_from_file(filename, &error);
	if (error || !images[i])
	    goto loadError;

	g_free(filename);
    }

  return TRUE;

  loadError:
  if (filename)
      g_free(filename);
  if (error->domain == GDK_PIXBUF_ERROR)
      g_print("Pixbuf Related Error:\n");
  if (error->domain == G_FILE_ERROR)
      g_print("File Error: Check file permissions and state:\n");
  g_printerr("%s\n", error[0].message);
  exit(1);
}

#define CYCLE_LEN 60

static int frame_num;

void *
drawFrame()
{
  double f;
  int i;
  double xmid, ymid;
  double radius;
  GdkPixbuf *frame;

  if (frame_num % 2)
      frame = frame1;
  else
      frame = frame2;

  gdk_pixbuf_copy_area (background, 0, 0, back_width, back_height,
                        frame, 0, 0);

  f = (double) (frame_num % CYCLE_LEN) / CYCLE_LEN;

  xmid = back_width / 2.0;
  ymid = back_height / 2.0;

  radius = MIN (xmid, ymid) / 2.0;

  for (i = 0; i < N_IMAGES; i++)
    {
      double ang;
      int xpos, ypos;
      int iw, ih;
      double r;
      GdkRectangle r1, r2, dest;
      double k;

      ang = 2.0 * G_PI * (double) i / N_IMAGES - f * 2.0 * G_PI;

      iw = gdk_pixbuf_get_width (images[i]);
      ih = gdk_pixbuf_get_height (images[i]);

      r = radius + (radius / 3.0) * sin (f * 2.0 * G_PI);

      xpos = floor (xmid + r * cos (ang) - iw / 2.0 + 0.5);
      ypos = floor (ymid + r * sin (ang) - ih / 2.0 + 0.5);

      k = (i & 1) ? sin (f * 2.0 * G_PI) : cos (f * 2.0 * G_PI);
      k = 2.0 * k * k;
      k = MAX (0.25, k);

      r1.x = xpos;
      r1.y = ypos;
      r1.width = iw * k;
      r1.height = ih * k;

      r2.x = 0;
      r2.y = 0;
      r2.width = back_width;
      r2.height = back_height;

      if (gdk_rectangle_intersect (&r1, &r2, &dest))
        gdk_pixbuf_composite (images[i],
                              frame,
                              dest.x, dest.y,
                              dest.width, dest.height,
                              xpos, ypos,
                              k, k,
                              GDK_INTERP_NEAREST,
                              ((i & 1)
                               ? MAX (127, fabs (255 * sin (f * 2.0 * G_PI)))
                               : MAX (127, fabs (255 * cos (f * 2.0 * G_PI)))));
    }

  frame_num++;
  return (void *)gdk_pixbuf_get_pixels(frame);
}

void
do_pixbufs(int w, int h)
{
    gtk_init(NULL, NULL);

    if (!load_pixbufs(w, h)) {
	g_print("Error loading pixmaps\n");
	exit(0);
    }

    frame1 = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE,
			    8, back_width, back_height);

    frame2 = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE,
			    8, back_width, back_height);
}
