#include <stdio.h>
#include "serial.h"
#include "protocol.h"

int read_byte(int fd, unsigned char *c)
{
	int res = read(fd,c,1);
	if (res != 1)
	{
		printf("Error reading from the serial port.\n");
		return 1;
	}
	printf("Read 0x%X\n", + *c);
	return 0;
}

int main(int argc, char** argv)
{
	if ( (argc < 2) || ((strcmp("/dev/ttyS0", argv[1])!=0) && (strcmp("/dev/ttyS4", argv[1])!=0) )) {
		printf("\n Usage:\tcommunicator SerialPort\n\tcentral: /dev/ttyS4\tTUX: /dev/ttyS0\n\n");
		exit(1);
	}

	int vtime = 0;
	int vmin = 1;
	int serial_fd = serial_initialize(argv[1], vmin, vtime);
	if(serial_fd < 0) exit(-1);

	typedef enum {START, FLAG_RCV, A_RCV, C_RCV, BCC_OK, STOP} state_t;
	state_t state = START;
	unsigned char byte;

	while(state != STOP) {
		
		int ret = read_byte(serial_fd, &byte);
		if(ret != 0) {
			perror("ERROR IN READ_BYTE\n");
			exit(-1);
		}
		
		switch(state) {
			case START:
				if(byte == FLAG)
					state = FLAG_RCV;
				break;
			case FLAG_RCV:
				if(byte == A)
					state = A_RCV;
				else if(byte != FLAG)
					state = START;
				break;
			case A_RCV:
				if(byte == FLAG)
					state = FLAG_RCV;
				else if(byte == C_SET)
					state = C_RCV;
				else
					state = START;
				break;
			case C_RCV:
				if(byte == FLAG)
					state = FLAG;
				else if(byte == (A ^ C_SET))
					state = BCC_OK;
				else
					state = START;
				break;
			case BCC_OK:
				if(byte == FLAG)
					state = STOP;
				else
					state = START;
				break;
			case STOP:
				break;
		}
	}

	printf("GOT SET\n");
	
	unsigned char UA[5];
	UA[0] = FLAG;
	UA[1] = A;
	UA[2] = C_UA;
	UA[3] = A ^ C_UA;
	UA[4] = FLAG;

	printf("SENT UA\n");	
	write(serial_fd, UA, 5);
	sleep(0.5);
	
	//res = read(serial_fd, message, bytes);
	//res = write(serial_fd, message, strlen(message));

	serial_terminate(serial_fd);

	close(serial_fd);
	return 0;
}
