#ifndef _protocol_h_
#define _protocol_h_

/*
 * Current protocol version number.  Increment subminor for bug fixes, minor
 * for new capabilities, and major for breaking backward compatibility.
 */
#define PROTOCOL_MAJOR    1
#define PROTOCOL_MINOR    0
#define PROTOCOL_SUBMINOR 0

/*
 * Protocol opcodes.
 */
#define DISPLAY			0x01
#define KEY_PRESSED		0x02
#define KEY_RELEASED		0x03
#define POINTER_PRESSED		0x04
#define POINTER_DRAGGED		0x05
#define POINTER_RELEASED	0x06
#define IMAGE_RGB32		0x07 /* variable length */
#define IMAGE_RGB565		0x08 /* variable length */
#define IMAGE_RGB24		0x09 /* variable length */
#define BLIT			0x0a 
#define FLUSH			0x0b 

/*
 * Length in 32-bit words of all fixed-length commands.
 */
#define DISPLAY_LENGTH          6
#define KEY_PRESSED_LENGTH      2
#define KEY_RELEASED_LENGTH     2
#define POINTER_PRESSED_LENGTH  2
#define POINTER_DRAGGED_LENGTH  2
#define POINTER_RELEASED_LENGTH 2
#define BLIT_LENGTH             4
#define FLUSH_LENGTH            1

/* longest fixed-length command */
#define CMD_FIXED_LENGTH_MAX    6

/* fixed-length portion of IMAGE commands */
#define IMAGE_HEADER_LENGTH     3

/* default port for socket connections */
#define DEFAULT_PORT 1301

/* extract opcode and header from command header */
#define GET_OPCODE(c) ((c) >> 24)
#define GET_LENGTH(c) ((c) & 0x00ffffff)

/* distinguish variable-length image commands */
#define IS_IMAGE(op) \
    ((op == IMAGE_RGB32) || (op == IMAGE_RGB565) || (op == IMAGE_RGB24))

#endif /* _protocol_h_ */
