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
	unsigned length;
	unsigned char bcc1;
	unsigned char bcc2;
	unsigned char address_field;
	unsigned char *buffer;
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
#define TRANSMITTER 0
#define RECEIVER 1

#define INIT_CONNECTION_TRIES 5
#define INIT_CONNECTION_RESEND_TIME 1

typedef struct {
	unsigned int fd;
	unsigned int tries_left;
	unsigned int time_dif;
	frame_t *frame;
	unsigned int *stop;
} alarm_info_t;

/*
 * Starts the connection via serial-port, allowing for it to be either reader or writer
 * Returns port fd, -1 if error
 */
int llopen(char *filename, int mode);

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
 * Returns >0 if success, <0 on error
 */
int llclose(int fd);

int byte_stuffing(const unsigned char *src, unsigned length, unsigned char **dst, unsigned *new_length);

/*
 * Writes given message a certain ammount of times with a delay between each of them, using alarms.
 * @param alrm_info_arg struct with alarm information
 * @return 0 if OK, > 0  otherwise
 */
int write_timed_frame(alarm_info_t *alrm_info_arg);
//int write_timed_message(int fd, char *msg, unsigned int len, unsigned int tries, unsigned int time_dif, unsigned int *stop_flag);

/*
 * Checks if stop flag is active or there are remaining tries, then writes the message again
 */
void alarm_handler();

int llopen_transmitter(int fd);

int llopen_receiver(int fd);

frame_t* get_frame(int fd);

int send_frame(int fd, const frame_t *frame);

int send_cmd_frame(int fd, const frame_t *frame);

int send_data_frame(int fd, const frame_t *frame);

#endif
