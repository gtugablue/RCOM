#include "application.h"
#include <stdio.h>

#define MAX(A, B) (((A) > (B)) ? (A) : (B))

int main(int argc, char *argv[]) // ./file_transfer <port> <send|receive> <filename>
{
	if (argc != 4)
	{
		printf("Usage: %s <port> <send|receive> <filename>\n", argv[0]);
		return 1;
	}

	return 0;
}

int send_file(const unsigned char *port, const unsigned char *file_name)
{
	FILE *fp = fopen(file_name, "r");
	fseek(fp, 0, SEEK_END);
	unsigned long size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	unsigned char data[size];
	fread(data, size, 1, fp);
	fclose(fp);

	int fd = llopen(port, TRANSMITTER);
	if (fd < 0) return 1;

	unsigned i;
	for (i = 0; i < size; i += MAX_PACKET_SIZE)
	{
		if (send_packet(fd, &data[i], MAX(MAX_PACKET_SIZE, size - i))) return 1;
	}

	return llclose(fd);
}

int send_packet(int fd, const unsigned char *data, unsigned size)
{
	if (llwrite(fd, &data[i], size) < 0) return 1;
	// TODO
}

int receive_file(const unsigned char *port, const unsigned char *destination_folder)
{

}
