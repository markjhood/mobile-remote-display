#ifndef _cmd_h_
#define _cmd_h_

#include "fb.h"
#include "stream.h"
#include "protocol.h"

void sendFlush(stream);
void sendDisplayEvent(stream, fb, int verbose);

/*
 * Send only the header of the IMAGE commands and return the number of
 * words of image data expected with the next send.  These calls should be
 * immediately followed by a call to streamSendCommand() with the pixel data.
 */
int sendImageRGB24Header(stream, int x, int y, unsigned int w, unsigned int h);
int sendImageRGB32Header(stream, int x, int y, unsigned int w, unsigned int h);
int sendImageRGB565Header(stream, int x, int y, unsigned int w, unsigned int h);

#endif /* _cmd_h_ */
