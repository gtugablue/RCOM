#ifndef DATALINK_H
#define DATALINK_H

/*
 * Describe function return values
 */
#define OK 0
#define ERROR -1

#define FLAG 126
#define A_TRANSMITTER 0x03
#define A_RECEIVER 0x01
#define C_SET 0x07
#define C_DISC 0x0B
#define C_UA 0x03
#define C_RR(n) ((n << 5) | 1)
#define C_REJ(n) ((n << 5) | 5)
#define C_INFO(n) (n << 5)

#define SET 0
#define UA 1
#define DATA 2

#define MAX_BUFFER_LENGTH 30

typedef struct {
	int length = 0;
	char buffer[MAX_BUFFER_LENGTH];
	int type;
} packet_t;

/*
 * Describes the receiver state-machine states
 */
typedef enum {START, FLAG_RCV, A_RCV, C_RCV, BCC_OK, STOP} state_t;

/*
 * Define the program mode (either reader or writer)
 */
#define TRANSMITTER 0
#define RECEIVER 1

/*
 * Starts the connection via serial-port, allowing for it to be either reader or writer
 * Returns port fd, -1 if error
 */
int llopen(int porta, int mode);

/*
 * Writes length bytes from buffer to fd
 * Return number of bytes written on success, -1 if error
 */
int llwrite(int fd, char * buffer, int length);

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

#endif
