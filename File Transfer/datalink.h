#ifndef DATALINK_H
#define DATALINK_H

/*
 * Describe function return values
 */
#define OK 0
#define ERROR -1

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
