#ifndef _fb_h_
#define _fb_h_

/*
 * Standard remote display window types and size definitions for current demos.
 * These are used as the windowType parameter in fbNew() and in the command
 * line interface.
 *
 * FB_SMALL_DASH -   640x480  car dashboard mockup
 * FB_LARGE_DASH -  1024x768  car dashboard mockup
 * FB_PRESENTATION - 800x600  unadorned presentation window
 * FB_NULL -           0x0    no window, no image buffer
 */
#define FB_NULL         0
#define FB_SMALL_DASH   1
#define FB_LARGE_DASH   2
#define FB_PRESENTATION 3

/*
 * The remote display window's dimensions.  The actual displayable area may be
 * smaller and offset by X, Y.
 */
#define FB_SMALL_DASH_WIDTH    640
#define FB_SMALL_DASH_HEIGHT   480

#define FB_LARGE_DASH_WIDTH   1024
#define FB_LARGE_DASH_HEIGHT   768

#define FB_PRESENTATION_WIDTH  800
#define FB_PRESENTATION_HEIGHT 600

#define FB_SMALL_DASH_DISPLAY_WIDTH  320
#define FB_SMALL_DASH_DISPLAY_HEIGHT 240
#define FB_SMALL_DASH_DISPLAY_XUL    130
#define FB_SMALL_DASH_DISPLAY_YUL    120

#define FB_LARGE_DASH_DISPLAY_WIDTH  600
#define FB_LARGE_DASH_DISPLAY_HEIGHT 400
#define FB_LARGE_DASH_DISPLAY_XUL    168
#define FB_LARGE_DASH_DISPLAY_YUL    184

#define FB_PRESENTATION_DISPLAY_WIDTH  800
#define FB_PRESENTATION_DISPLAY_HEIGHT 600
#define FB_PRESENTATION_DISPLAY_XUL      0
#define FB_PRESENTATION_DISPLAY_YUL      0

/*
 * Remote display image formats.  The pixels are formatted as documented for
 * the corresponding protocol opcodes.  FB_IMAGE_NULL is a special case
 * indicating there's no image to draw.
 */
#define FB_IMAGE_NULL   0
#define FB_IMAGE_RGB32  1
#define FB_IMAGE_RGB24  2
#define FB_IMAGE_RGB565 3

/*
 * These display attribute values match MIDP3 constants.
 */

/* display hardware state */
#define FB_DISPLAY_HARDWARE_ENABLED  0
#define FB_DISPLAY_HARDWARE_DISABLED 1
#define FB_DISPLAY_HARDWARE_ABSENT   2

/* display status */
#define FB_STATE_BACKGROUND 0
#define FB_STATE_VISIBLE    1
#define FB_STATE_FOREGROUND 2

/* orientation */
#define FB_ORIENTATION_PORTRAIT      1
#define FB_ORIENTATION_LANDSCAPE     2
#define FB_ORIENTATION_PORTRAIT_180  4
#define FB_ORIENTATION_LANDSCAPE_180 8

/*
 * Display capabilities are bit values and match MIDP3.
 */
#define FB_SUPPORTS_INPUT_EVENTS                 1
#define FB_SUPPORTS_COMMANDS                     2
#define FB_SUPPORTS_FORMS                        4
#define FB_SUPPORTS_TICKER                       8
#define FB_SUPPORTS_TITLE                       16
#define FB_SUPPORTS_ALERTS                      32
#define FB_SUPPORTS_LISTS                       64
#define FB_SUPPORTS_TEXTBOXES                  128
#define FB_SUPPORTS_TABBEDPANES                256
#define FB_SUPPORTS_FILESELECTORS              512
#define FB_SUPPORTS_MENUS                     1024
#define FB_SUPPORTS_IDLEITEM                  2048
#define FB_SUPPORTS_ORIENTATION_PORTRAIT      4096
#define FB_SUPPORTS_ORIENTATION_LANDSCAPE     8192
#define FB_SUPPORTS_ORIENTATION_PORTRAIT180  16384
#define FB_SUPPORTS_ORIENTATION_LANDSCAPE180 32768

typedef struct _fb *fb;

fb fbNew(unsigned int windowType, unsigned int verbose);
fb fbNewWithSize(unsigned int windowWidth, unsigned int windowHeight,
		 int displayX, int displayY, unsigned int displayWidth,
		 unsigned int displayHeight, unsigned int verbose);

void fbFree(fb);
void fbClear(fb);

/*
 * Clear the framebuffer and set the image format to FB_IMAGE_NULL to indicate
 * that the image is invalid and should be ignored.
 */
void fbImageNull(fb);

int fbDisplayX(fb);
int fbDisplayY(fb);

unsigned int fbDisplayWidth(fb);
unsigned int fbDisplayHeight(fb);
unsigned int fbWindowWidth(fb);
unsigned int fbWindowHeight(fb);

unsigned int fbDisplayDotPitch(fb);
unsigned int fbDisplayOrientation(fb);

void *fbImage(fb);
unsigned int fbImageSize(fb);
unsigned int fbImageFormat(fb);

unsigned int fbHardwareState(fb);
unsigned int fbDisplayStatus(fb);
unsigned int fbDisplayCapabilities(fb);

void fbTestImageRGB32(fb);
void fbTestImageRGB24(fb);

/*
 * Set an arbitrary 24-bit background image for the frame buffer.
 */
void fbBgImageRGB24(fb, int x, int y, int w, int h, unsigned char *pixels);

/*
 * These always get the entire command, including the header that contains the
 * opcode and length.  The length always refers to the length of the entire
 * command in 32-bit words.
 */
void fbReadImageRGB32Request(fb, unsigned int *cmd, int length);
void fbReadImageRGB24Request(fb, unsigned int *cmd, int length);
void fbReadImageRGB565Request(fb, unsigned int *cmd, int length);
void fbReadBlitRequest(fb, unsigned int *cmd, int length);
void fbReadState(fb, unsigned int *cmd, int length);

/*
 * Set framebuffer to the given hwState / dpyStatus.
 */
void fbSetState(fb, int hwState, int dpyStatus);

/*
 * Call the appropriate fb function to handle `opcode', using the command and
 * associated data referenced by `in'.
 */
void fbDispatch(fb, unsigned char opcode, unsigned int *in, int length);

#endif /* _fb_h_ */
