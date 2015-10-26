#ifndef __FRAME_VALIDATOR_H
#define __FRAME_VALIDATOR_H

#include "datalink.h"

int invalid_frame(const frame_t *frame);

int check_bcc1(const frame_t *frame);
int check_bcc2(const frame_t *frame);

#endif //__FRAME_VALIDATOR_H
