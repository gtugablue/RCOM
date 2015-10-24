#ifndef __FRAME_VALIDATOR_H
#define __FRAME_VALIDATOR_H

#include "datalink.h"

int valid_frame(frame_t *frame);

int valid_data_frame(frame_t *frame);

int valid_cmd_frame(frame_t *frame);

#endif //__FRAME_VALIDATOR_H
