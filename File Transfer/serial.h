#ifndef SERIAL_H
#define SERIAL_H

/*Non-Canonical Input Processing*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BAUDRATE B38400
#define MODEMDEVICE "/dev/ttyS1"
#define _POSIX_SOURCE 1 /* POSIX compliant source */

#define DEFAULT_VMIN 1
#define DEFAULT_VTIME 30

/*
 Open serial port device for reading and writing and not as controlling tty
 because we don't want to get killed if linenoise sends CTRL-C.
 */
int serial_initialize(const char *serial_port, int vmin, int vtime, int baudrate);
int serial_terminate(int fd);

#endif
