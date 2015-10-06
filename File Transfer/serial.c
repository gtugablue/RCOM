#include "serial.h"

struct termios oldtio;	// TO ALLOW serial_terminate TO WORK

int serial_initialize(char *serial_port, int vmin, int vtime) {
	struct termios newtio;
	int fd = open(serial_port, O_RDWR | O_NOCTTY );
	if (fd <0) {
		perror(serial_port);
		return -1;
	}

	if ( tcgetattr(fd,&oldtio) == -1) { /* save current port settings */
		perror("tcgetattr");
		return -1;
	}

	bzero(&newtio, sizeof(newtio));
	newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;
	newtio.c_iflag = IGNPAR;
	newtio.c_oflag = 0;

	/* set input mode (non-canonical, no echo,...) */
	newtio.c_lflag = 0;

	newtio.c_cc[VTIME]    = vtime;   /* inter-character timer unused */
	newtio.c_cc[VMIN]     = vmin;   /* blocking read until VMIN chars received */

	/* 
	 VTIME e VMIN devem ser alterados de forma a proteger com um temporizador a 
	 leitura do(s) próximo(s) caracter(es)
	*/

	tcflush(fd, TCIOFLUSH);

	if ( tcsetattr(fd,TCSANOW,&newtio) == -1) {
		perror("tcsetattr");
		return -1;
	}

	printf("New termios structure set\n");

	return fd;
}

int serial_terminate(int fd) {
	if ( tcsetattr(fd,TCSANOW,&oldtio) == -1) {
		perror("tcsetattr");
		return -1;
	}
	return 0;
}
