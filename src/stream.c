#include <stdlib.h>
#include <strings.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <signal.h>

#include "stream.h"
#include "protocol.h"

struct _stream {
    /* the socket file descriptor */
    int socket;

    /* one buffer for stream input, and another for byte swapping sends */
    unsigned int *in;
    unsigned int *out;
    unsigned int length;

    /* IMAGE commands can be sent in several pieces */
    unsigned int cmdWordsLeft;

    /* if non-zero print some info to stdout */
    int verbose;

#ifdef PERF_OUTPUT
    /* performance monitoring support, if enabled and verbose */
    struct timeval t, t0, lastSend, lastRead;
    long long bytesSent, totalBytesSent;
    long long bytesRead, totalBytesRead;
    long long framesRead, totalFramesRead;
    long long usecs;
#endif /* PERF_OUTPUT */
};

static void 
error(char *msg)
{
    perror(msg);
    exit(1);
}

/*
 * Initiate a connection through the given socket to the specified IP address.
 * For the demo we just keep trying until we get through.
 */
static int 
doConnect(int sock, char *ipaddr, struct sockaddr *p, int verbose)
{
    struct sockaddr_in *addr = (struct sockaddr_in *)p;
    switch(inet_pton(AF_INET, ipaddr, &addr->sin_addr)) {
	case 0:
	    fprintf(stderr, "doConnect(): bad IP address %s.\n", ipaddr);
	    exit(1);
	case -1:
	    error(ipaddr);
    }

    while (connect(sock, (struct sockaddr *)addr, sizeof(*addr)) < 0) {
	if (verbose) perror("connect() failed, retrying...");
	sleep(2);
	/*
	 * We don't need to close the current socket and recreate it when
	 * running under Ubuntu 8.04, but if we don't do this for Mac OS X
	 * 10.6 then the subsequent calls to connect() will fail with "Invalid
	 * argument" and never make a connection.
	 */
	close(sock);
	sock = socket(PF_INET, SOCK_STREAM, 0);
    }

    return sock;
}

/*
 * Listen for incoming connections on the socket and return the resulting data
 * stream.
 */
static int 
doListen(int socket, struct sockaddr *p)
{
    int yes, streamfd;
    struct sockaddr_in remote_addr;
    struct sockaddr_in *addr = (struct sockaddr_in *)p;
    socklen_t length;

    /*
     * Allow other sockets to bind() to the port we're going to use so that
     * we can avoid "address in use" errors when restarting.
     */
    yes = 1;
    if (setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0) {
	error("setsockopt() error.");
    }

    /* Bind to our own IP address with port already set up in addr */
    addr->sin_addr.s_addr = INADDR_ANY;
    if (bind(socket, (struct sockaddr *)addr, sizeof(*addr)) < 0) 
	error("bind() error.");

    /* Use the socket to listen for a connection on the port */
    if (listen(socket, 5) < 0)
	error("listen() error.");

    /* Wait for an incoming connection and get the new socket */
    length = sizeof(remote_addr);
    streamfd = accept(socket, (struct sockaddr *)&remote_addr, &length);
    if (streamfd < 0) 
	error("accept() error.");

    /* Don't need the listening socket anymore */
    close(socket);
    return streamfd;
}

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
stream
streamNew(int length, int verbose)
{
    stream s = (stream)malloc(sizeof(struct _stream));
    if (!s) {
	fprintf(stderr, "streamNew(): failed to allocate stream structure.");
	exit(1);
    }

    /* allocate in and out buffers */
    s->length = length;
    s->in = (unsigned int *)malloc(s->length * 4);
    if (!s->in) {
	fprintf(stderr, "streamNew(): failed allocating %d input words.\n",
		length);
	exit(1);
    }

    s->out = (unsigned int *)malloc(s->length * 4);
    if (!s->out) {
	fprintf(stderr, "streamNew(): failed allocating %d output words.\n",
		length);
	exit(1);
    }

    s->socket = -1;
    s->cmdWordsLeft = 0;
    s->verbose = verbose;
    return s;
}

/*
 * Set up an Internet TCP stream on the specified port.  If an IP address is
 * provided, then initiate a connection to that address; otherwise, listen for
 * incoming connections.
 */
void
streamOpen(stream s, char *ipaddr, int port)
{
    int socketfd, streamfd;
    struct sockaddr_in addr;

    if (s->socket != -1) {
	fprintf(stderr, "streamOpen(): stream is already open.\n");
	exit(1);
    }

    /* ignore SIGPIPE errors */
    signal(SIGPIPE, SIG_IGN);

    /* Create a socket for an Internet TCP stream */
    socketfd = socket(PF_INET, SOCK_STREAM, 0);
    if (socketfd < 0)
	error("socket() error.");

    /* Set up common parts of the IP address info struct */
    bzero((char *)&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    /* connect or listen */
    if (ipaddr) {
	streamfd = doConnect(socketfd, ipaddr, (struct sockaddr *)&addr,
			     s->verbose);
    } else {
	streamfd = doListen(socketfd, (struct sockaddr *)&addr);
    }

    s->socket = streamfd;

#ifdef PERF_OUTPUT
    s->bytesSent = s->totalBytesSent = 0;
    s->bytesRead = s->totalBytesRead = 0;
    s->framesRead = s->totalFramesRead = 0;

    gettimeofday(&s->t0, NULL);
    s->lastSend = s->lastRead = s->t0;
#endif
}

void
streamClose(stream s)
{
    if (s->socket != -1) {
	close(s->socket);
	s->socket = -1;
    } else {
	fprintf(stderr, "streamClose(): stream is already closed.\n");
	exit(1);
    }
}

void
streamFree(stream s)
{
    if (s->socket != -1)
	streamClose(s);
    
    if (s->in) {
	free(s->in);
    } else {
	fprintf(stderr, "streamFree(): input buffer already free.\n");
	exit(1);
    }
    
    if (s->out) {
	free(s->out);
    } else {
	fprintf(stderr, "streamFree(): output buffer already free.\n");
	exit(1);
    }

    s->length = 0;
    free(s);
}

int
streamSocket(stream s)
{
    return s->socket;
}

/*
 * Possibly byte swap host data and copy to the output buffer.  The cmd buffer
 * comes from client data and must not be modified.
 */
static void
htonlv(unsigned int cmd[], unsigned int out[], int length)
{
    while (length--) {
	*out = htonl(*cmd);
	cmd++; out++;
    }
}

/*
 * Possibly swap network data and overwrite the input buffer.
 */
static void
ntohlv(unsigned int cmd[], int length)
{
    while (length--) {
	*cmd = ntohl(*cmd);
	cmd++;
    }
}

#ifdef PERF_OUTPUT
#define ELAPSED(l, c) \
    (((c.tv_sec - l.tv_sec) * 1000000) + (c.tv_usec - l.tv_usec))

/* sample interval in microseconds */
#define INTERVAL 2000000

/* max string length for performance monitoring */
#define LEN 100

static char *
rate(long long bytes, long long usecs)
{
    static char s[LEN];
    double rate = (double)bytes/((double)usecs/1000000.0);

    if (rate < 10000)
	snprintf(s, LEN, "%lld bytes/sec", (long long)rate);
    else if (rate < 1000000)
	snprintf(s, LEN, "%.1f kbytes/sec", rate/1024.0);
    else
	snprintf(s, LEN, "%.1f mbytes/sec", rate/(1024.0*1024.0));

    return s;
}
#endif /* PERF_OUTPUT */

static void
sendWords(stream s, unsigned int *cmd, int length)
{
    int n; int bytesLeft = length * 4;
    unsigned char *p = (unsigned char *)cmd;

    while (bytesLeft) {
	n = send(s->socket, p, bytesLeft, 0);
	if (n < 0) {
	    /*
	     * Don't exit the progam; peer may have dropped the connection
	     * uncleanly.  We'll wait for another connection.  Note: under
	     * OSX we get n < 0, but on Linux we get a SIGPIPE and we die
	     * unless we call signal(SIGPIPE, SIG_IGN) to ignore it.
	     */
	    if (s->verbose) perror("send() error.");
	    return;
	}
	p += n;
	bytesLeft -= n;
    }
}

/*
 * Send a command consisting of `length' 32-bit words through the socket.
 *
 * IMAGE commands may be transmitted in several calls to this function,
 * typically first with the fixed length command header, and then with the
 * pixel data, which is never byte-swapped.  The command header must be one
 * contiguous block.
 */
void
streamSendCommand(stream s, unsigned int *cmd, int length)
{
    if (s->cmdWordsLeft) {
	sendWords(s, cmd, length);
	s->cmdWordsLeft -= length;
    } else {
	unsigned char op = GET_OPCODE(cmd[0]);
	if (length > s->length) {
	    fprintf(stderr, "streamSendCommand(): length %d > max %d.\n",
		    length, s->length);
	    exit(1);
	}
	if (IS_IMAGE(op)) {
	    /* swap command, but not pixels, from host to network byte order */
	    htonlv(cmd, s->out, IMAGE_HEADER_LENGTH);
	    sendWords(s, s->out, IMAGE_HEADER_LENGTH);
	    sendWords(s, cmd + IMAGE_HEADER_LENGTH,
		      length - IMAGE_HEADER_LENGTH);
	} else {
	    /* byte swap the entire command */
	    htonlv(cmd, s->out, length);
	    sendWords(s, s->out, length);
	}
	s->cmdWordsLeft = GET_LENGTH(cmd[0]) - length;
    }

#ifdef PERF_OUTPUT
    if (s->verbose) {
	s->bytesSent += length * 4;
	gettimeofday(&s->t, NULL);
	if ((s->usecs = ELAPSED(s->lastSend, s->t)) > INTERVAL) {
	    s->totalBytesSent += s->bytesSent;

	    printf("Send rate: cur %s ", rate(s->bytesSent, s->usecs));
	    printf("avg %s ", rate(s->totalBytesSent, ELAPSED(s->t0, s->t)));
	    printf("tot %lld bytes.\n", s->totalBytesSent);

	    s->bytesSent = 0;
	    s->lastSend = s->t;
	}
    }
#endif
}

/*
 * Read a command from the stream's socket.  Returns the opcode read, or
 * STREAM_DISCONNECT if the other end disconnected.  *cmd gets a pointer to
 * the received command and data, and *length gets the length in 32-bit words.
 */
unsigned char
streamReadCommand(stream s, unsigned int **cmd, int *length)
{
    unsigned int *in = s->in;
    unsigned char opcode, *p;
    int n, l, byteLength, count;

    if (!in) {
	fprintf(stderr, "streamReadCommand():  input buffer is null.\n");
	exit(1);
    }

    /* read the cmd header: 4 bytes */
    n = recv(s->socket, in, 4, 0);
    if (n == 0) {
	if (s->verbose) printf("Peer closed the connection.\n");
	streamClose(s);
	return STREAM_DISCONNECT;
    }
    else if (n < 0) {
	error("recv() error.");
    }

    /* convert from network to host byte order */
    in[0] = ntohl(in[0]);

    /* get the opcode and length */
    opcode = GET_OPCODE(in[0]);
    l = GET_LENGTH(in[0]);

    if (l > s->length) {
	fprintf(stderr, "readCommand(): length %d > max %d, opcode 0x%x.\n",
		l, s->length, opcode);
	exit(1);
    }

    /* skip past the header and read all the data */
    p = (unsigned char *)in + 4;
    byteLength = l * 4;

    for (count = 4; count < byteLength; count += n, p += n) {
	n = recv(s->socket, p, byteLength - count, 0);
	if (n == 0) {
	    if (s->verbose) printf("Peer closed the connection.\n");
	    streamClose(s);
	    return STREAM_DISCONNECT;
	}
	else if (n < 0) {
	    error("recv() error.");
	}
    }
    
    /* convert data from network to host byte order */
    ntohlv(in + 1, IS_IMAGE(opcode) ? IMAGE_HEADER_LENGTH - 1 : l - 1);

#ifdef PERF_OUTPUT
    if (s->verbose) {
	s->bytesRead += byteLength;
	if (opcode == FLUSH)
	    s->framesRead++;

	gettimeofday(&s->t, NULL);
	if ((s->usecs = ELAPSED(s->lastRead, s->t)) > INTERVAL) {
	    s->totalBytesRead += s->bytesRead;
	    s->totalFramesRead += s->framesRead;

	    printf("Read rate: cur %s ", rate(s->bytesRead, s->usecs));
	    printf("avg %s ", rate(s->totalBytesRead, ELAPSED(s->t0, s->t)));
	    printf("tot %lld bytes\n", s->totalBytesRead);
	    printf("fps: cur %.1f avg %.1f.\n",
		   (double)s->framesRead / ((double)s->usecs / 1000000.0),
		   (double)s->totalFramesRead /
		   ((double)ELAPSED(s->t0, s->t) / 1000000.0));
	       
	    s->bytesRead = 0;
	    s->framesRead = 0;	       
	    s->lastRead = s->t;
	}
    }
#endif

    *cmd = in;
    *length = l;
    return opcode;
}
