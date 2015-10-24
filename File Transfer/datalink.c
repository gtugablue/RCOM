#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include "datalink.h"
#include "serial.h"
#include "frame_validator.h"

alarm_info_t alrm_info;
void alarm_handler() {
	if(alrm_info.stop != NULL && *alrm_info.stop) {
		alrm_info.tries_left = 0;
		return;
	}

	if(alrm_info.tries_left > 0) {
		alrm_info.tries_left--;
		if(send_frame(alrm_info.fd, alrm_info.frame)) {
			printf("ERROR (alarm_handler): unable to send requested frame");
			return;
		}
		alarm(alrm_info.time_dif);
	} else if(alrm_info.stop != NULL && !(*alrm_info.stop)) {
		if(kill(getpid(), SIGINT) != 0) {
			printf("ERROR (alarm_handler): unable to send SIGINT signal.");
			return;
		}
	}
}

int write_timed_frame(alarm_info_t alrm_info_arg) {

	if(alrm_info_arg.frame == NULL || alrm_info_arg.tries_left == 0 || alrm_info_arg.time_dif == 0) {
		printf("ERROR (write_timed_frame): invalid alarm info parameters.");
		return 1;
	}

	alrm_info = alrm_info_arg;
	signal(SIGALRM, alarm_handler);
	if(send_frame(alrm_info.fd, alrm_info.frame))
		return 1;
	alarm(alrm_info.time_dif);

	return 0;
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

int llopen(char *filename, int mode) {
	int vtime = 30;
	int vmin = 0;
	int serial_fd = serial_initialize(filename, vmin, vtime);
	if (serial_fd < 0) return -1;

	switch(mode) {
	case TRANSMITTER:
		if(llopen_transmitter(serial_fd))
			return -1;
		break;
	case RECEIVER:
		if(llopen_receiver(serial_fd))
			return -1;
		break;
	default:
		printf("ERROR (llopen): invalid serial port opening mode.");
		return -1;
	}

	return serial_fd;
}

int llopen_transmitter(int fd) {
	frame_t frame;
	frame.sequence_number = 0;
	frame.length = 1;
	frame.buffer = malloc(sizeof(char));
	frame.buffer[0] = C_SET;
	frame.type = CMD_FRAME;

	if(send_frame(fd, &frame)) {
		printf("ERROR (llopen_transmitter): unable to send SET.");
		return 1;
	}

	frame_t *answer = get_frame(fd);
	if(invalid_frame(&frame) || answer->buffer[2] != C_UA) {
		printf("ERROR (llopen_transmitter): received invalid frame. Expected valid UA command frame");
		return 1;
	}

	return 0;
}

int llopen_receiver(int fd) {

	int attempts = INIT_CONNECTION_TRIES;

	while (attempts-- > 0) {
		frame_t *frame = get_frame(fd);

		if(invalid_frame(frame) || frame->buffer[2] != C_SET) {
			printf("ERROR (llopen_receiver): received invalid frame. Expected valid SET command frame");
			return 1;
		} else {
			break;
		}
	}

	frame_t answer;
	answer.sequence_number = 0;
	answer.length = 1;
	answer.buffer = malloc(sizeof(char));
	answer.buffer[0] = C_UA;
	answer.type = CMD_FRAME;

	if(send_frame(fd, &answer)) {
		printf("ERROR (llopen_receiver): unable to answer sender's SET.");
		return 1;
	}

	return 0;
}

int llwrite(int fd, const unsigned char *buffer, int length) {
	frame_t frame;
	frame.sequence_number = 0;
	if ((frame.buffer = malloc(length)) == NULL) return 1;
	memcpy(frame.buffer, buffer, length);
	if (send_data_frame(fd, &frame)) return 1;
	return 0;
}

int llread(int fd, char * buffer) {
	return 0;
}

int llclose(int fd) {
	return 0;
}

int send_frame(int fd, const frame_t *frame) {
	switch(frame->type) {
	case CMD_FRAME:
		return send_cmd_frame(fd, frame);
	case DATA_FRAME:
		return send_data_frame(fd, frame);
	}

	printf("ERROR (send_frame): invalid frame type received");
	return 1;
}

int send_cmd_frame(int fd, const frame_t *frame)
{
	if(frame->buffer == NULL || frame->length != 1)
		return 1;

	unsigned char msg[] = {FLAG,
			A_TRANSMITTER,
			frame->buffer[0],
			A_TRANSMITTER ^ frame->buffer[0],
			FLAG};
	if (write(fd, msg, sizeof(msg)) != sizeof(msg)) return 1;

	return 0;
}

int send_data_frame(int fd, const frame_t *frame) // TODO UNTESTED
{
	unsigned char ctrl = frame->sequence_number << 5;
	unsigned char fh[] = {FLAG,
			A_TRANSMITTER,
			ctrl,
			A_TRANSMITTER ^ ctrl
	};

	unsigned char *data;
	unsigned length;
	if (byte_stuffing(frame->buffer, frame->length, &data, &length)) return 1;

	int i;
	unsigned char bcc2;
	if (length > 0)
	{
		bcc2 = data[0];
		for (i = 1; i < length; ++i)
		{
			bcc2 ^= data[i];
		}
	}
	else bcc2 = 0;
	unsigned char *bcc2_stuffed;
	unsigned length2;
	if (byte_stuffing(&bcc2, sizeof(bcc2), &bcc2_stuffed, &length2)) return 1;

	unsigned char ft[] = {bcc2_stuffed,
			FLAG
	};

	if (write(fd, fh, sizeof(fh)) != sizeof(fh)) return 1;
	if (write(fd, data, length) != length) return 1;
	if (write(fd, bcc2_stuffed, length2) != length2) return 1;
	if (write(fd, ft, sizeof(ft)) != sizeof(ft)) return 1;

	free(data);
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
			break;
		case DATA_RCV:
			frame->buffer[frame->length++] = byte;
			state = DATA_ESC_RCV;
			break;
		case STOP:
			break;
		}
	}

	return frame;
}

/*
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
