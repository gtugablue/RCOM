#ifndef APPLICATION_H
#define APPLICATION_H

int send_file(const unsigned char *port, const unsigned char *file_name);
int receive_file(const unsigned char *port, const unsigned char *destination_folder);

#endif
