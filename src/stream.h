#ifndef _stream_h_
#define _stream_h_

typedef struct _stream *stream;

/*
 * Allocate a stream structure with input and output buffers of `length'
 * words.  If `verbose' is non-zero, print some info to stdout as we run.
 *
 * IMAGE commands can be sent in several pieces; only the header of length
 * IMAGE_HEADER_LENGTH words needs to be contiguous.  If the stream is not
 * used to read IMAGE commands, then a length of CMD_FIXED_LENGTH_MAX is
 * sufficient for sending and receiving all other commands.
 *
 * If the stream will be used for reading IMAGE commands then `length' must be
 * long enough to accomodate the largest expected image + IMAGE_HEADER_LENGTH.
 */
stream streamNew(int length, int verbose);
void streamFree(stream);

/*
 * Set up an Internet TCP stream on the specified port.  If an IP address is
 * provided, then initiate a connection to that address; otherwise, listen for
 * incoming connections.
 */
void streamOpen(stream, char *ipaddr, int port);
void streamClose(stream);
int streamSocket(stream);

/*
 * Send a command consisting of `length' 32-bit words through the socket.
 *
 * IMAGE commands may be transmitted in several calls to this function,
 * typically first with the fixed length command header, and then with the
 * pixel data, which is never byte-swapped.  The command header must be one
 * contiguous block.
 */
void streamSendCommand(stream, unsigned int *cmd, int length);

/*
 * Read a command from the stream's socket.  Returns the opcode read, or
 * STREAM_DISCONNECT if the other end disconnected.  *cmd gets a pointer to
 * the received command and data, and *length gets the length in 32-bit words.
 */
unsigned char streamReadCommand(stream, unsigned int **cmd, int *length);

/*
 * Returned by streamReadCommand() in addition to the opcodes defined in
 * protocol.h
 */
#define STREAM_DISCONNECT 0

#endif /* _stream_h_ */
