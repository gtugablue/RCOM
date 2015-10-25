#ifndef APPLICATION_H
#define APPLICATION_H

#include "datalink.h"

#define MAX_PACKET_SIZE	1024

int send_file(const char *port, const char *file_name);
int receive_file(const char *port, const char *destination_folder);

#endif
