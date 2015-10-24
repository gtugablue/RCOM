#ifndef __FRAME_VALIDATOR_H
#define __FRAME_VALIDATOR_H

#include "datalink.h"

int invalid_frame(const frame_t *frame);

int invalid_data_frame(const frame_t *frame);

int invalid_cmd_frame(const frame_t *frame);

#endif //__FRAME_VALIDATOR_H
