#ifndef APPLICATION_H
#define APPLICATION_H

#define MAX_PACKET_SIZE	124

int send_file(const char *port, const char *file_name);
int receive_file(const char *port, const char *destination_folder);

#endif
