#include "application.h"
#include <stdio.h>
#include <stdint.h>

#define MAX(A, B) (((A) > (B)) ? (A) : (B))

typedef enum {
	PACKET_CTRL_FIELD_DATA = 0,
	PACKET_CTRL_FIELD_START = 1,
	PACKET_CTRL_FIELD_END = 2
} packet_ctrl_field_t;

typedef struct {
	unsigned char type;
	unsigned char length;
	unsigned char *value;
} control_packet_param_t;

typedef struct {
	unsigned char ctrl_field;
	unsigned char sn;
	uint16_t length;
	unsigned char *data;
} data_packet_t;

typedef struct {
	unsigned char ctrl_field;
	unsigned num_params;
	control_packet_param_t *params;
} control_packet_t;

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

int send_data_packet(int fd, const data_packet_t *data_packet)
{
	unsigned size = data_packet->length + 4;
	unsigned char packet[size];
	packet[0] = data_packet->control_field;
	packet[1] = data_packet->sn;
	packet[2] = data_packet->length & 0xFF00;
	packet[3] = data_packet->length & 0x00FF;
	memcpy(&packet[4], data_packet->data, length);
	//if (llwrite(fd, packet, size) < 0) return 1;
	printf("Cenas: %s\n", packet);
}

int receive_file(const unsigned char *port, const unsigned char *destination_folder)
{

}
