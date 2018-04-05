/*
 * This program is used to test the remote display program.
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>

#include "cmd.h"
#include "pixbufs.h"

static void
usage()
{
    fprintf(stderr, "usage: demotest [-port <n>] [-connect <ipaddr>]\n");
    fprintf(stderr, "  The port should be a number between 1023 and 65536.\n");
    fprintf(stderr, "If no port is supplied then %d is used.\n", DEFAULT_PORT);
    fprintf(stderr, "  The default is to listen for an incoming connection\n");
    fprintf(stderr, "on the specified port.  If an IP address is supplied\n");
    fprintf(stderr, "with the '-connect' option then a connection will be\n");
    fprintf(stderr, "made to the specified host instead.\n");
    fprintf(stderr, "  Options can be abbreviated.  If only one argument is\n");
    fprintf(stderr, "provided then it is taken as the port number.\n");
    exit(0);
}

int 
main(int argc, char *argv[])
{
    fb f;
    stream s;
    char *ipaddr;
    int port, verbose;

    ipaddr = 0;
    port = DEFAULT_PORT;
    verbose = 1;

    while (--argc > 0 && (*++argv)[0] == '-') {
	if (--argc < 1)
	    /* all options must be followed by values */
	    usage();
	switch (*++(argv[0])) {
	    case 'p':
	    case 'P':
		port = atoi(*++argv);
		break;
	    case 'c':
	    case 'C':
		ipaddr = *++argv;
		break;
	    default:
		usage();
	}
    }
    switch (argc) {
	case 1:
	    /* interpret single arg as port number */
	    port = atoi(*argv);
	case 0:
	    break;
	default:
	    usage();
    }
    if (port < 1024 || port > 65535)
	usage();

    if (ipaddr)
	printf("Connecting to %s on port %d\n", ipaddr, port);
    else
	printf("Listening on port %d\n", port);

    /* initialize the stream and framebuffer APIs */
    f = fbNew(FB_NULL, verbose);
    s = streamNew(CMD_FIXED_LENGTH_MAX, verbose);

    /* loop until we're killed */
    while (1) {
	fd_set fds;
	unsigned int *cmd;
	int length, connected;
	struct timeval timeout = {0, 0};

	/* set up for select(2) */
	FD_ZERO(&fds);

	/* block until we make a connection */
	streamOpen(s, ipaddr, port);
	connected = 1;

	/*
	 * Get the 1st event from remote. It must be a DISPLAY event that
	 * will give us the remote display metrics and state.
	 */
	if (streamReadCommand(s, &cmd, &length) != DISPLAY) {
	    fprintf(stderr, "Did not get expected DISPLAY event.\n");
	    exit(1);
	}

	/* Get the display info */
	fbReadState(f, cmd, length);

	/* Set up the demo pixmaps */
	do_pixbufs(fbDisplayWidth(f), fbDisplayHeight(f));

	/* loop until we get disconnected */
	while (connected) {
	    int socket;
	    char opcode, *pixels;

	    /* draw a frame and send it to the remote display */
	    pixels = drawFrame();
	    length = sendImageRGB24Header
		(s, 0, 0, fbDisplayWidth(f), fbDisplayHeight(f));
	    streamSendCommand(s, (unsigned int *)pixels, length);
	    sendFlush(s);

	    /*
	     * Poll for read data on the stream.  We used to use the
	     * asynchronous SIGIO mechanism, but that's not supported on
	     * sockets in Mac OS X!
	     */
	    socket = streamSocket(s);
	    FD_SET(socket, &fds);
	    select(socket+1, &fds, NULL, NULL, &timeout);

	    if (FD_ISSET(socket, &fds)) {
		/* read data is available */
		switch (opcode = streamReadCommand(s, &cmd, &length)) {
		    case STREAM_DISCONNECT:
			streamClose(s);
			connected = 0;
			break;
		    case DISPLAY:
			fbReadState(f, cmd, length);
			if (fbHardwareState(f) == FB_DISPLAY_HARDWARE_ABSENT) {
			    streamClose(s);
			    connected = 0;
			}
			break;
		    default:
			fprintf(stderr, "Got unexpected opcode %d.\n", opcode);
			exit(1);
			break;
		}
	    }
	} /* while connected loop */
    } /* while alive loop */
}
