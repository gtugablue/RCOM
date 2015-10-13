#include "datalink.c"

typedef enum {
	START,
	FLAG_RCV,
	A_RCV,
	C_RCV,
	BCC_OK,
	STOP
} state_t;

/*int read_packet(int fd, unsigned char C) // TODO
{
	state_t state = START;
	unsigned char byte;

	while(state != STOP) {

		int ret = read_byte(fd, &byte);
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
			else if(byte == C)
				state = C_RCV;
			else
				state = START;
			break;
		case C_RCV:
			if(byte == FLAG)
				state = FLAG;
			else if(byte == (A ^ C))
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
}

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
*/
