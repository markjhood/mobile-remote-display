#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fb.h"
#include "protocol.h"

struct _fb {
    /* the image buffer */
    unsigned int *buf;
    unsigned int imageSize;
    unsigned int imageFormat;

    /* display hardware state */
    unsigned int hardwareState;

    /* display status */
    unsigned int displayStatus;

    /* display capabilities */
    unsigned int displayCapabilities;

    /* display metrics */
    int displayX;
    int displayY;
    unsigned int displayWidth;
    unsigned int displayHeight;
    unsigned int displayDotPitch;
    unsigned int displayOrientation;

    /* window width and height */
    unsigned int windowWidth;
    unsigned int windowHeight;

    /* if non-zero print info to stdout as we run */
    unsigned int verbose;
};

static void 
error(char *msg)
{
    perror(msg);
    exit(0);
}

fb
fbNew(unsigned int windowType, unsigned int verbose)
{
    switch (windowType) {
	case FB_NULL:
	    /*
	     * XXX TODO - meh.  Should separate the image buffer from the
	     * remote display state.  Sometimes the DISPLAY command is all
	     * we're interested in.
	     */
	    return fbNewWithSize(0, 0, 0, 0, 0, 0, verbose);
	case FB_SMALL_DASH:
	    return fbNewWithSize(FB_SMALL_DASH_WIDTH,
				 FB_SMALL_DASH_HEIGHT,
				 FB_SMALL_DASH_DISPLAY_XUL,
				 FB_SMALL_DASH_DISPLAY_YUL,
				 FB_SMALL_DASH_DISPLAY_WIDTH,
				 FB_SMALL_DASH_DISPLAY_HEIGHT,
				 verbose);
	case FB_LARGE_DASH:
	    return fbNewWithSize(FB_LARGE_DASH_WIDTH,
				 FB_LARGE_DASH_HEIGHT,
				 FB_LARGE_DASH_DISPLAY_XUL,
				 FB_LARGE_DASH_DISPLAY_YUL,
				 FB_LARGE_DASH_DISPLAY_WIDTH,
				 FB_LARGE_DASH_DISPLAY_HEIGHT,
				 verbose);
	case FB_PRESENTATION:
	    return fbNewWithSize(FB_PRESENTATION_WIDTH,
				 FB_PRESENTATION_HEIGHT,
				 FB_PRESENTATION_DISPLAY_XUL,
				 FB_PRESENTATION_DISPLAY_YUL,
				 FB_PRESENTATION_DISPLAY_WIDTH,
				 FB_PRESENTATION_DISPLAY_HEIGHT,
				 verbose);
	default:
	    fprintf(stderr, "fbNew(): unknown window type %d.\n", windowType);
	    exit(1);
    }
}

fb
fbNewWithSize(unsigned int windowWidth, unsigned int windowHeight,
	      int offsetX, int offsetY, unsigned int displayWidth,
	      unsigned int displayHeight, unsigned int verbose)
{
    fb f = (fb)malloc(sizeof(struct _fb));
    if (!f) error("failed to allocate framebuffer structure");

    f->windowWidth = windowWidth;
    f->windowHeight = windowHeight;
    f->displayX = offsetX;
    f->displayY = offsetY;
    f->displayWidth = displayWidth;
    f->displayHeight = displayHeight;
    f->verbose = verbose;

    f->imageSize = displayWidth * displayHeight;
    if (f->imageSize) {
	f->buf = (unsigned int *)malloc(f->imageSize * 4);
	if (!f->buf) error("failed to allocate framebuffer storage");
    } else {
	/* FB_NULL */
	f->buf = 0;
    }

    f->imageFormat = FB_IMAGE_NULL;
//  fbTestImageRGB32(f);
//  fbTestImageRGB24(f);

    /*
     * Set nominal state.  `hardwareState' and `displayStatus' are set when
     * the client connects to the remote display.
     */
    f->displayOrientation = FB_ORIENTATION_LANDSCAPE;

    /*
     * This is specific to the Lenovo T60 14.1" screen used for the demo.
     * 286mm/1400pixels = 204um/pixel. Not sure if this is used by anything.
     */
    f->displayDotPitch = 204;

    /*
     * Support all except FB_INPUT_EVENTS, FB_ORIENTATION_PORTRAIT,
     * FB_ORIENTATION_PORTRAIT180, FB_ORIENTATION_LANDSCAPE180.
     */
    f->displayCapabilities =
	FB_SUPPORTS_COMMANDS      |  
	FB_SUPPORTS_FORMS         |
	FB_SUPPORTS_TICKER        |
	FB_SUPPORTS_TITLE         |
	FB_SUPPORTS_ALERTS        |
	FB_SUPPORTS_LISTS         |
	FB_SUPPORTS_TEXTBOXES     |
	FB_SUPPORTS_TABBEDPANES   |
	FB_SUPPORTS_FILESELECTORS |
	FB_SUPPORTS_MENUS         |
	FB_SUPPORTS_IDLEITEM      |
	FB_SUPPORTS_ORIENTATION_LANDSCAPE;

    return f;
}


void
fbFree(fb f)
{
    if (f->buf) {
	free(f->buf);
	f->buf = 0;
    }
    f->imageSize = 0;
    f->imageFormat = FB_IMAGE_NULL;
    free(f);
}

void 
fbClear(fb f)
{
    if (f->buf) {
	memset((void *)f->buf, 0, f->imageSize * 4);
	f->imageFormat = FB_IMAGE_RGB32;
    }
}

/*
 * Clear the framebuffer and set the image format to FB_IMAGE_NULL to indicate
 * that the image is invalid and should be ignored.
 */
void
fbImageNull(fb f)
{
    fbClear(f);
    f->imageFormat = FB_IMAGE_NULL;
}

int
fbDisplayX(fb f)
{
    return f->displayX;
}

int
fbDisplayY(fb f)
{
    return f->displayY;
}

unsigned int
fbDisplayWidth(fb f)
{
    return f->displayWidth;
}

unsigned int
fbDisplayHeight(fb f)
{
    return f->displayHeight;
}

unsigned int
fbWindowWidth(fb f)
{
    return f->windowWidth;
}

unsigned int
fbWindowHeight(fb f)
{
    return f->windowHeight;
}

unsigned int
fbDisplayDotPitch(fb f)
{
    return f->displayDotPitch;
}

unsigned int
fbDisplayOrientation(fb f)
{
    return f->displayOrientation;
}

void *
fbImage(fb f)
{
    return (void *)f->buf;
}

unsigned int
fbImageSize(fb f)
{
    return f->imageSize;
}

unsigned int
fbImageFormat(fb f)
{
    return f->imageFormat;
}

unsigned int
fbHardwareState(fb f)
{
    return f->hardwareState;
}

unsigned int
fbDisplayStatus(fb f)
{
    return f->displayStatus;
}

unsigned int
fbDisplayCapabilities(fb f)
{
    return f->displayCapabilities;
}

void
fbTestImageRGB32(fb f)
{
    unsigned int *p;
    int pixelCount, bandLength;

    if (!f->buf) return;

    p = f->buf;
    bandLength = f->displayWidth * f->displayHeight / 4;

    /* displays red on little-endian machines */
    pixelCount = bandLength;
    while (pixelCount--) *p++ = 0x000000ff;

    /* displays green on little-endian machines */
    pixelCount = bandLength;
    while (pixelCount--) *p++ = 0x0000ff00;

    /* displays blue on little-endian machines */
    pixelCount = bandLength;
    while (pixelCount--) *p++ = 0x00ff0000;

    /*
     * Displays black on little-endian machines.
     * 
     * XXX TODO - Alpha is ignored as padding by gdk_draw_rgb_32_image().  May
     * need to call something like gdk_pixbuf_composite() if we decide we need
     * to support Porter-Duff compositing or similar on remote display.
     */
    pixelCount = bandLength;
    while (pixelCount--) *p++ = 0xff000000;

    f->imageFormat = FB_IMAGE_RGB32;
}

void
fbTestImageRGB24(fb f)
{
    unsigned char *p;
    int pixelCount, bandLength;

    if (!f->buf) return;

    p = (unsigned char *)f->buf;
    bandLength = f->displayWidth * f->displayHeight / 3;

    /* displays red */
    pixelCount = bandLength;
    while (pixelCount--) {
	p[0] = 0xff; p[1] = 0x00; p[2] = 0x00;
	p += 3;
    }

    /* displays green */
    pixelCount = bandLength;
    while (pixelCount--) {
	p[0] = 0x00; p[1] = 0xff; p[2] = 0x00;
	p += 3;
    }

    /* displays blue */
    pixelCount = bandLength;
    while (pixelCount--) {
	p[0] = 0x00; p[1] = 0x00; p[2] = 0xff;
	p += 3;
    }

    f->imageFormat = FB_IMAGE_RGB24;
}

/*
 * Set an arbitrary 24-bit background image for the frame buffer.
 */
void
fbBgImageRGB24(fb f, int x, int y, int w, int h, unsigned char *pixels)
{
    unsigned char *p;
    unsigned int stride;
    
    if (!f->buf) return;
    stride = f->displayWidth;

    p = (unsigned char *)f->buf + 3 * (x + y * stride);
    w *= 3;
    stride *= 3;
    
    while (h--) {
	memcpy(p, pixels, w);
	p += stride;
	pixels += w;
    }

    f->imageFormat = FB_IMAGE_RGB24;
}

#define GETxywh(b) { \
    b++;             \
    x = *b >> 16; y = *b & 0x0000ffff; b++; \
    w = *b >> 16; h = *b & 0x0000ffff; b++; \
}

void 
fbReadImageRGB32Request(fb f, unsigned int *b, int length)
{
    /*
     * XXX - TODO: clipping!
     */
    int x, y, w, h, l;
    unsigned int *p, stride;

    if (!f->buf) return;
    GETxywh(b);

    stride = f->displayWidth;
    p = f->buf + x + y * stride;

    l = w * 4;
    while (h--) {
	memcpy(p, b, l);
	p += stride;
	b += w;
    }

    f->imageFormat = FB_IMAGE_RGB32;
}

void 
fbReadImageRGB24Request(fb f, unsigned int *b, int length)
{
    /*
     * XXX - TODO: clipping!
     */
    int x, y, w, h;
    unsigned char *p, *pb;
    unsigned int stride;
    
    if (!f->buf) return;

    GETxywh(b);
    stride = f->displayWidth;

    p = (unsigned char *)f->buf + 3 * (x + y * stride);
    pb = (unsigned char *)b;

    w *= 3;
    stride *= 3;
    
    while (h--) {
	memcpy(p, pb, w);
	p += stride;
	pb += w;
    }

    f->imageFormat = FB_IMAGE_RGB24;
}

void 
fbReadImageRGB565Request(fb f, unsigned int *b, int length)
{
    /*
     * XXX - TODO: clipping!
     */
    int x, y, w, h, i;
    unsigned char *p, *pfb;
    unsigned short *ps;
    unsigned int stride;
    
    if (!f->buf) return;

    GETxywh(b);
    stride = f->displayWidth;

    pfb = (unsigned char *)f->buf + 3 * (x + y * stride);
    ps = (unsigned short *)b;

    stride *= 3;
    while (h--) {
	p = pfb;
	for (i = 0; i < w; i++) {
	    *p++ = (*ps & 0xf800) >> 8; /* red */
	    *p++ = (*ps & 0x07e0) >> 3; /* green */
	    *p++ = (*ps & 0x001f) << 3; /* blue */
	    ps++;
	}
	pfb += stride;
    }

    /* image has been converted to RGB24 */
    f->imageFormat = FB_IMAGE_RGB24;
}

void 
fbReadBlitRequest(fb f, unsigned int *b, int length)
{
    if (!f->buf) return;
    if (f->verbose)
	printf("fbReadBlitRequest() unimplemented\n");
}

/*
 * Read framebuffer / remote display state from a DISPLAY command buffer
 * received from the host.  The `in' buffer must be at least DISPLAY_LENGTH
 * words long.
 */
void 
fbReadState(fb f, unsigned int *in, int length)
{
    int major, minor, subminor;

    if (length < DISPLAY_LENGTH) {
	fprintf(stderr, "fbReadState(): length %d < min length %d.\n",
		length, DISPLAY_LENGTH);
	exit(1);
    }

    major =    (in[1] & 0x00ff0000) >> 16;
    minor =    (in[1] & 0x0000ff00) >>  8;
    subminor = (in[1] & 0x000000ff) >>  0;

    if (f->verbose) {
	printf("fbReadState():\n");
	printf("  read protocol version: %d.%d.%d:",
	       major, minor, subminor);
	printf("  build version is %d.%d.%d\n",
	       PROTOCOL_MAJOR, PROTOCOL_MINOR, PROTOCOL_SUBMINOR);
    }

    f->hardwareState =       in[2] >> 16;
    f->displayStatus =       in[2] & 0x0000ffff;
    f->displayCapabilities = in[3];
    f->displayWidth =        in[4] >> 16;
    f->displayHeight =       in[4] & 0x0000ffff;
    f->displayDotPitch =     in[5] >> 16;
    f->displayOrientation =  in[5] & 0x0000ffff;

    if (f->verbose) {
	switch (f->hardwareState) {
	    case FB_DISPLAY_HARDWARE_ENABLED:
		printf("  hardware state: ENABLED");
		break;
	    case FB_DISPLAY_HARDWARE_DISABLED:
		printf("  hardware state: DISABLED");
		break;
	    case FB_DISPLAY_HARDWARE_ABSENT:
		printf("  hardware state: ABSENT");
		break;
	    default:
		printf("  hardware state: %d (UNKNOWN)", f->hardwareState);
		break;
	}
	switch (f->displayStatus) {
	    case FB_STATE_BACKGROUND:
		printf("  display status: BACKGROUND\n");
		break;
	    case FB_STATE_VISIBLE:
		printf("  display status: VISIBLE\n");
		break;
	    case FB_STATE_FOREGROUND:
		printf("  display status: FOREGROUND\n");
		break;
	}
	printf("  display capabilities: 0x%08x\n", f->displayCapabilities);
	printf("  width %d height %d dot pitch %d um/pixel\n",
	       f->displayWidth, f->displayHeight, f->displayDotPitch);

	switch (f->displayOrientation) {
	    case FB_ORIENTATION_PORTRAIT:
		printf("  orientation PORTRAIT\n");
		break;
	    case FB_ORIENTATION_LANDSCAPE:
		printf("  orientation LANDSCAPE\n");
		break;
	    case FB_ORIENTATION_PORTRAIT_180:
		printf("  orientation PORTRAIT_180\n");
		break;
	    case FB_ORIENTATION_LANDSCAPE_180:
		printf("  orientation LANDSCAPE_180\n");
		break;
	    default:
		printf("  orientation UNKNOWN\n");
		break;
	}
    }
}

/*
 * Set fb to the given hwState / dpyStatus.
 */
void 
fbSetState(fb f, int hwState, int dpyStatus)
{
    f->hardwareState = hwState;
    f->displayStatus = dpyStatus;
}

/*
 * Call the appropriate fb function to handle `opcode', using the command and
 * associated data referenced by `in'.  This is used in conjunction with
 * streamReadCommand().
 */
void
fbDispatch(fb f, unsigned char opcode, unsigned int *in, int length)
{
    switch (opcode) {
	case DISPLAY:
	    fbReadState(f, in, length);
	    break;
	case IMAGE_RGB32:
	    fbReadImageRGB32Request(f, in, length);
	    break;
	case IMAGE_RGB24:
	    fbReadImageRGB24Request(f, in, length);
	    break;
	case IMAGE_RGB565:
	    fbReadImageRGB565Request(f, in, length);
	    break;
	case BLIT:
	    fbReadBlitRequest(f, in, length);
	    break;
	case FLUSH:
	    break;
	default:
	    fprintf(stderr, "fbDispatch(): unknown opcode 0x%x.\n", opcode);
	    exit(1);
	    break;
    }
}
