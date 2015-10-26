#include <stdio.h>
#include "frame_validator.h"

int check_bcc1(const frame_t *frame);
int check_bcc2(const frame_t *frame);
int invalid_data_frame(const frame_t *frame);
int invalid_cmd_frame(const frame_t *frame);

int invalid_frame(const frame_t *frame) {
	if(frame == NULL) return 1;

	switch(frame->type) {
	case DATA_FRAME:
		return invalid_data_frame(frame);
	case CMD_FRAME:
		return invalid_cmd_frame(frame);
	default:
		printf("ERROR (valid_frame): invalid frame type %d.", frame->type);
		return 1;
	}

	return 1;
}

int invalid_data_frame(const frame_t *frame) {
	return check_bcc1(frame) || check_bcc2(frame);
}

int invalid_cmd_frame(const frame_t *frame) {
	return check_bcc1(frame);
}

int check_bcc1(const frame_t *frame) {
	if((frame->control_field ^ frame->address_field) != frame->bcc1) {
		return 1;
	}

	return 0;
}

int check_bcc2(const frame_t *frame) {
	if(frame->length <= 0)
		return 1;

	unsigned i;
	char cumulative_xor = frame->buffer[0];

	for(i = 1; i < frame->length; i++) {
		cumulative_xor = (cumulative_xor ^ frame->buffer[i]);
	}

	if(cumulative_xor == frame->bcc2)
		return 0;

	return 1;
}
