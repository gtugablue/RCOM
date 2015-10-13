#ifndef DATALINK_H
#define DATALINK_H

/*
 * Describe function return values
 */
#define OK 0
#define ERROR 1

/*
 * Define the program mode (either reader or writer)
 */
#define LLOPEN_READ 0
#define LLOPEN_WRITE 1

/*
 * Starts the connection via serial-port, allowing for it to be either reader or writer
 */
int llopen(int port_fd, int mode);



#endif
