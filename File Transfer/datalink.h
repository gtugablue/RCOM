#ifndef __DATALINK_H
#define __DATALINK_H

#include <stdlib.h>

#define BIT(n) (1 << n)

#define ORDER_BIT(n) (n << 5)

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
#define LLREAD_ANSWER_TRIES 5
#define LLREAD_ANSWER_RESEND_TIME 1
#define LLREAD_VALIDMSG_TRIES 5

typedef struct {
	int fd;
	unsigned int tries_left;
	unsigned int time_dif;
	frame_t *frame;
	unsigned int stop;
} alarm_info_t;

typedef enum {
	FIRST,
	MIDDLE,
	LAST
} frame_order_t;

typedef struct {
	int fd;
	int mode;
	unsigned int curr_seq_number;
	unsigned int repeat;
	frame_order_t frame_order;
} datalink_t;

/*
 * Initializes all datalink parameters except fd(set to -1)
 */
void datalink_init(datalink_t *datalink, unsigned int mode);

/*
 * Starts the connection via serial-port, allowing for it to be either reader or writer
 * Returns 0 if OK, > 0 otherwise
 */
int llopen(char *filename, datalink_t *datalink);

/*
 * Writes length bytes from buffer to fd
 * Return number of bytes written on success, -1 if error
 */
int llwrite(datalink_t *datalink, const unsigned char *buffer, int length);

/*
 * Reads from fd to buffer
 * Returns buffer size if ok, -1 if error
 */
int llread(datalink_t *datalink, char * buffer);

/*
 * Closes fd data link
 * Returns 0 if success, <0 on error
 */
int llclose(datalink_t *datalink);


#endif
