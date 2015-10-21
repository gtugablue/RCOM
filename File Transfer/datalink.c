#include "datalink.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

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

int llopen(int porta, int mode) {
	return 0;
}

int llwrite(int fd, const unsigned char *buffer, int length) {
	frame_t frame;
	frame.sequence_number = 0;
	if ((frame.buffer = malloc(length)) == NULL) return 1;
	memcpy(frame.buffer, buffer, length);
	if (write_frame(fd, &frame)) return 1;
	return 0;
}

int llread(int fd, char * buffer) {
	return 0;
}

int llclose(int fd) {
	return 0;
}

int write_frame(int fd, const frame_t *frame) // UNTESTED
{
	char msg = FLAG;
	if (write(fd, &msg, 1) != 1) return 1;
	msg = A_TRANSMITTER;
	if (write(fd, &msg, 1) != 1) return 1;
	if (write(fd, (unsigned char *)(frame->sequence_number), 1) != 1) return 1;
	msg = (char)(A_TRANSMITTER ^ frame->sequence_number);
	if (write(fd, &msg, 1) != 1) return 1;

	unsigned char *stuffed;
	unsigned length;
	if (byte_stuffing(frame->buffer, frame->length, &stuffed, &length)) return 1;

	if (write(fd, stuffed, length) != length) return 1;

	int i;
	unsigned char bcc2 = stuffed[0];
	for (i = 1; i < length; ++i)
	{
		bcc2 ^= stuffed[i];
	}
	free(stuffed);

	msg = bcc2;
	if (write(fd, (char*)&msg, 1) != 1) return 1;
	msg = FLAG;
	if (write(fd, (char*)&msg, 1) != 1) return 1;
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

frame_t* get_frame(int fd) {
	state_t state = START;
	unsigned char byte;
	frame_t *frame = (frame_t *)malloc(sizeof(frame_t));

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
				frame->buffer[frame->length++] = byte;
			}
			break;
		case FLAG_RCV:
			if(byte == A_TRANSMITTER || byte == A_RECEIVER) {	// TODO check
				state = A_RCV;
				frame->buffer[frame->length++] = byte;
			} else if(byte != FLAG) {
				state = START;
				frame->length = 0;
			}
			break;
		case A_RCV:
			if(byte == FLAG) {
				frame->length = 1;
				state = FLAG_RCV;
			} else if(byte == C_SET) {
				frame->type = SET;
				frame->buffer[frame->length++] = byte;
				state = C_RCV;
			} else if(byte == C_UA) {
				frame->type = UA;
				frame->buffer[frame->length++] = byte;
				state = C_RCV;
			} else if(byte == C_DATA(0)  || byte == C_DATA(1)) {
				frame->type = DATA;
				frame->buffer[frame->length++] = byte;
				state = C_RCV;
			} else {
				frame->length = 0;
				state = START;
			}
			break;
		case C_RCV:
			if(byte == (frame->buffer[1] ^ frame->buffer[2])) {
				if(frame->type == SET || frame->type == UA)
					state = BCC1_RCV;
				else
					state = DATA_ESC_RCV;

				frame->buffer[frame->length++] = byte;
			} else if (byte == FLAG){
				state = FLAG_RCV;
				frame->length = 1;
			} else {
				frame->length = 0;
				state = START;
			}
			break;
		case BCC1_RCV:
			if(byte == FLAG) {
				frame->buffer[frame->length++] = byte;
				state = STOP;
			} else {
				frame->length = 0;
				state = START;
			}
			break;
		case DATA_ESC_RCV:
			if(byte == ESC) {
				state = DATA_RCV;
			} else if(byte == FLAG) {
				frame->buffer[frame->length++] = byte;
				//if(bcc2_checks(&frame)) {
					state = STOP;
				//}
			} else {
				frame->buffer[frame->length++] = byte;
			}
		case DATA_RCV:
			frame->buffer[frame->length++] = byte;
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
