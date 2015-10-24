#include "frame_validator.h"

int valid_frame(frame_t *frame) {
	if(frame == NULL) return 1;

	switch(frame->type) {
	case DATA_FRAME:
		return valid_data_frame(frame);
	case CMD_FRAME:
		return valid_cmd_frame(frame);
	default:
		printf("ERROR (valid_frame): invalid frame type %d.", frame->type);
		return 1;
	}

	return 1;
}

int valid_data_frame(frame_t *frame) {
	// TODO
	return 0;
}

int valid_cmd_frame(frame_t *frame) {
	if(frame->length != MSG_CMD_LENGTH)
		return 1;

	if(frame->buffer[0] != FLAG  || frame->buffer[4] != FLAG)
		return 1;

	char a_field = frame->buffer[1];
	if(a_field != A_TRANSMITTER && a_field != A_RECEIVER)
		return 1;

	char c_field = frame->buffer[2];
	char possibilities[] = {C_SET, C_DISC, C_UA, C_RR(0), C_RR(1), C_REJ(0), C_REJ(1)};
	int i, lim = sizeof(possibilities);
	for(i = 0; i < lim; ++i) {
		if(possibilities[i] == c_field) {
			break;
		}
	}
	if(i >= lim)
		return 1;

	if(frame->buffer[3] != (a_field ^ c_field))
		return 1;

	return 0;
}
