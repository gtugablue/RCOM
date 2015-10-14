#include "datalink.h"
#include <stdlib.h>

int llopen(int porta, int mode) {

}

int llwrite(int fd, char * buffer, int length) {
	unsigned i;
	for (i = 0; i < length; ++i)
	{

	}
}

int llread(int fd, char * buffer) {

}

int llclose(int fd) {

}

int write_frame(int fd, frame_t frame) // UNTESTED
{
	if (write(fd, FLAG, 1) != 1) return 1;
	if (write(fd, A_TRANSMITTER, 1) != 1) return 1;
	if (write(fd, frame.sequence_number, 1) != 1) return 1;
	if (write(fd, A_TRANSMITTER ^ frame.sequence_number, 1) != 1) return 1;

	unsigned char *stuffed;
	unsigned length;
	if (byte_stuffing(&frame.buffer, frame.length, &stuffed, &length)) return 1;

	if (write(fd, stuffed, length) != length) return 1;

	int i;
	unsigned char bcc2 = stuffed[0];
	for (i = 1; i < length; ++i)
	{
		bcc2 ^= stuffed[i];
	}

	if (write(fd, bcc2, 1) != 1) return 1;
	if (write(fd, FLAG, 1) != 1) return 1;
	return 0;
}

int byte_stuffing(const unsigned char *src, unsigned length, unsigned char **dst, unsigned *new_length)
{
	unsigned char stuffed[2 * length];
	unsigned i;
	unsigned j = 0;
	for (i = 0, j = 0; i < length; ++i, ++j)
	{
		if (src[i] == FLAG)
		{
			stuffed[j] = ESC;
			stuffed[++j] = FLAG ^ ESC_XOR;
		}
		else if (src[i] == ESC)
		{
			stuffed[j] = ESC;
			stuffed[++j] = ESC ^ ESC_XOR;
		}
		else
			stuffed[j] = src[i];
	}
	if ((*dst = malloc(j - 1)) == NULL) return 1;
	memcpy(*dst, stuffed, j - 1);
	return 0;
}

char* get_frame(int fd) {
	state_t state = START;
	unsigned char byte;
	frame_t frame;

	while(state != STOP) {
		int ret = read_byte(fd, &byte);
		if(ret != 0) {
			perror("ERROR READING FROM SERIAL PORT\n");
			exit(-1);
		}

		switch(state) {
		case START:
			if(byte == FLAG) {
				state = FLAG_RCV;
				frame.buffer[frame.length++] = byte;
			}
			break;
		case FLAG_RCV:
			if(byte == A_TRANSMITTER || byte == A_RECEIVER) {	// TODO check
				state = A_RCV;
				frame.buffer[frame.length++] = byte;
			} else if(byte != FLAG) {
				state = START;
				frame.length = 0;
			}
			break;
		case A_RCV:
			if(byte == flag) {
				frame.length = 1;
				state = FLAG_RCV;
			} else if(byte == C_SET) {
				frame.type = SET;
				frame.buffer[frame.length++] = byte;
				state = C_RCV;
			} else if(byte == C_UA) {
				frame.type = UA;
				frame.buffer[frame.length++] = byte;
				state = C_RCV;
			} else if(byte == C_INFO(0)  || byte == C_INFO(1)) {
				frame.type = DATA;
				frame.buffer[frame.length++] = byte;
				state = C_RCV;
			} else {
				frame.length = 0;
				state = START;
			}
			break;
		case C_RCV:
			if(byte == frame.buffer[1] ^ frame.buffer[2]) {
				if(frame.type == SET || frame.type == UA)
					state = BCC1_RCV;
				else
					state = DATA_ESC_RCV;

				frame.buffer[frame.length++] = byte;
			} else if (byte == FLAG){
				state = FLAG_RCV;
				frame.length = 1;
			} else {
				frame.length = 0;
				state = START;
			}
			break;
		case BCC1_RCV:
			if(byte == FLAG) {
				frame.buffer[frame.length++] = byte;
				state = STOP;
			} else {
				frame.length = 0;
				state = START;
			}
			break;
		case DATA_ESC_RCV:
			if(byte == ESC) {
				state = DATA_RCV;
			} else if(byte == FLAG) {
				frame.buffer[frame.length++] = byte;
				if(bcc2_checks(&frame)) {
					state = stop;
				}
			} else {
				frame.buffer[frame.length++] = byte;
			}
		case DATA_RCV:
			frame.buffer[frame.length++] = byte;
			state = DATA_ESC_RCV;
		case STOP:
			break;
		}
	}

	return frame;
}

/*int read_frame(int fd, unsigned char C) // TODO
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
