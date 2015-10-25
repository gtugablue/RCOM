#ifndef __DATALINK_H
#define __DATALINK_H

#include <stdlib.h>

/*
 * Describe function return values
 */
#define OK 0
#define ERROR -1

#define FLAG 126
#define ESC 0x7D
#define ESC_XOR 0x20
#define A_TRANSMITTER 0x03
#define A_RECEIVER 0x01
#define C_DATA(S) ((S) << 5)
#define C_SET 0x07
#define C_DISC 0x0B
#define C_UA 0x03
#define C_RR(R) (((R) << 5) | 1)
#define C_REJ(R) (((R) << 5) | 5)

#define SET 0
#define UA 1
#define DATA 2

#define MAX_BUFFER_LENGTH 256

typedef enum {
	CMD_FRAME,
	DATA_FRAME
} frame_type_t;

typedef struct {
	unsigned char sequence_number;
	unsigned char address_field;
	unsigned char control_field;
	unsigned char bcc1;
	unsigned length;
	unsigned char *buffer;
	unsigned char bcc2;
	frame_type_t type;
} frame_t;

/*
 * Describes the receiver state-machine states
 */
typedef enum {START,
	FLAG_RCV,
	A_RCV,
	C_RCV,
	BCC1_RCV,
	DATA_ESC_RCV,
	DATA_RCV,
	STOP
} state_t;

/*
 * Define the program mode (either reader or writer)
 */
#define SENDER 0	// TODO enum
#define RECEIVER 1

#define INIT_CONNECTION_TRIES 5
#define INIT_CONNECTION_RESEND_TIME 1
#define FINAL_DISCONNECTION_TRIES 5
#define FINAL_DISCONNECTION_RESEND_TIME 1

typedef struct {
	int fd;
	unsigned int tries_left;
	unsigned int time_dif;
	frame_t *frame;
	unsigned int stop;
} alarm_info_t;

typedef struct {
	int fd;
	int mode;
	unsigned int curr_seq_number;
	unsigned int repeat;
} datalink_t;

/*
 * Starts the connection via serial-port, allowing for it to be either reader or writer
 * Returns port fd, -1 if error
 */
int llopen(const char *filename, int mode);

/*
 * Writes length bytes from buffer to fd
 * Return number of bytes written on success, -1 if error
 */
int llwrite(int fd, const unsigned char * buffer, int length);

/*
 * Reads from fd to buffer
 * Returns buffer size if ok, -1 if error
 */
int llread(int fd, char * buffer);

/*
 * Closes fd data link
 * Returns 0 if success, <0 on error
 */
int llclose(int fd, int mode);


#endif
