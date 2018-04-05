#include <stdio.h>
#include <math.h>
#include "cmd.h"

void
sendFlush(stream s)
{
    unsigned int flushCmd = (FLUSH << 24) | (0x00ffffff & FLUSH_LENGTH);
    streamSendCommand(s, &flushCmd, FLUSH_LENGTH);
}

void
sendDisplayEvent(stream s, fb f, int verbose)
{
    unsigned int dpyCmd[DISPLAY_LENGTH];

    if (verbose) {
	printf("sendDisplayEvent(): fd %d hwState: ", streamSocket(s));
	switch (fbHardwareState(f)) {
	    case FB_DISPLAY_HARDWARE_ENABLED:
		printf("ENABLED ");
		break;
	    case FB_DISPLAY_HARDWARE_DISABLED:
		printf("DISABLED ");
		break;
	    case FB_DISPLAY_HARDWARE_ABSENT:
		printf("ABSENT ");
		break;
	    default:
		printf("unknown ");
		break;

	}
	printf("dpyStatus: ");
	switch (fbDisplayStatus(f)) {
	    case FB_STATE_BACKGROUND:
		printf("BACKGROUND\n");
		break;
	    case FB_STATE_VISIBLE:
		printf("VISIBLE\n");
		break;
	    case FB_STATE_FOREGROUND:
		printf("FOREGROUND\n");
		break;
	    default:
		printf("unknown\n");
		break;
	}
    }

    dpyCmd[0] = (DISPLAY << 24) | (0x00ffffff & DISPLAY_LENGTH);
    dpyCmd[1] = PROTOCOL_MAJOR << 16 | PROTOCOL_MINOR << 8 | PROTOCOL_SUBMINOR;
    dpyCmd[2] = fbHardwareState(f) << 16 | fbDisplayStatus(f);
    dpyCmd[3] = fbDisplayCapabilities(f);
    dpyCmd[4] = fbDisplayWidth(f) << 16 | fbDisplayHeight(f);
    dpyCmd[5] = fbDisplayDotPitch(f) << 16 | fbDisplayOrientation(f);

    streamSendCommand(s, dpyCmd, DISPLAY_LENGTH);
}

static void
sendImageHeader(stream s, char op, int length, int x, int y,
		unsigned int w, unsigned int h)
{
    unsigned int imgCmd[IMAGE_HEADER_LENGTH];

    imgCmd[0] = (op << 24) | (0x00ffffff & (length + IMAGE_HEADER_LENGTH));
    imgCmd[1] =  (x << 16) | (0x0000ffff & y);
    imgCmd[2] =  (w << 16) | (0x0000ffff & h);

    streamSendCommand(s, imgCmd, IMAGE_HEADER_LENGTH);
}

/*
 * Send only the header of the IMAGE commands and return the number of
 * words of image data expected with the next send.  These calls should be
 * immediately followed by a call to streamSendCommand() with the pixel data.
 */
int
sendImageRGB24Header(stream s, int x, int y, unsigned int w, unsigned int h)
{
    int imageSize = ceil(w * h * 3.0 / 4.0);
    sendImageHeader(s, IMAGE_RGB24, imageSize, x, y, w, h);
    return imageSize;
}

int
sendImageRGB32Header(stream s, int x, int y, unsigned int w, unsigned int h)
{
    int imageSize = w * h;
    sendImageHeader(s, IMAGE_RGB32, imageSize, x, y, w, h);
    return imageSize;
}

int
sendImageRGB565Header(stream s, int x, int y, unsigned int w, unsigned int h)
{
    int imageSize = ceil(w * h / 2.0);
    sendImageHeader(s, IMAGE_RGB565, imageSize, x, y, w, h);
    return imageSize;
}
