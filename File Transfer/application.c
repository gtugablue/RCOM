#include "application.h"
#include "datalink.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define MAX(A, B) (((A) > (B)) ? (A) : (B))
#define MIN(A, B) (((A) < (B)) ? (A) : (B))

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
	packet_ctrl_field_t ctrl_field;
	unsigned char sn;
	uint16_t length;
	unsigned char *data;
} data_packet_t;

typedef struct {
	packet_ctrl_field_t ctrl_field;
	unsigned num_params;
	control_packet_param_t *params;
} control_packet_t;

int send_data_packet(datalink_t *datalink, const data_packet_t *data_packet);

int main(int argc, char *argv[]) // ./file_transfer <port> <send|receive> <filename>
{
	if (argc != 4)
	{
		printf("Usage: %s <port> <send|receive> <filename>\n", argv[0]);
		return 1;
	}
	datalink_t datalink;
	if(strcmp(argv[2], "send") == 0) {
		datalink_init(&datalink, SENDER);
		if (send_file(argv[1], argv[3])) return 1;
	} else if(strcmp(argv[2], "receive") == 0) {
		datalink_init(&datalink, RECEIVER);
		llopen(argv[1], &datalink);
		unsigned char buf[1000000];
		printf("Reading...\n");
		llread(&datalink, buf);
		printf("Read done.\n");
		// TODO
	}
	return 0;
}

int send_file(const char *port, const char *file_name)
{

	FILE *fp = fopen(file_name, "r");
	fseek(fp, 0, SEEK_END);
	unsigned long size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	unsigned char data[size];
	fread(data, size, 1, fp);
	fclose(fp);

	datalink_t datalink;
	datalink.mode = SENDER;
	if (llopen(port, &datalink)) return 1;

	unsigned long i;
	for (i = 0; i < size; i += MAX_PACKET_SIZE)
	{
		data_packet_t data_packet;
		data_packet.ctrl_field = PACKET_CTRL_FIELD_DATA;
		data_packet.sn = i;
		data_packet.length = MIN(MAX_PACKET_SIZE, size - i);
		data_packet.data = &data[i];
		if (send_data_packet(&datalink, &data_packet)) return 1;
	}

	return llclose(&datalink);
}

int send_data_packet(datalink_t *datalink, const data_packet_t *data_packet)
{
	unsigned size = data_packet->length + 4;
	unsigned char packet[size];
	packet[0] = data_packet->ctrl_field;
	packet[1] = data_packet->sn;
	packet[2] = (uint8_t)((data_packet->length & 0xFF00) >> 8);
	packet[3] = (uint8_t)(data_packet->length & 0x00FF);
	memcpy(&packet[4], data_packet->data, size);
	if (llwrite(datalink, packet, size) < 0) return 1;

	return 0;
}

int receive_file(const char *port, const char *destination_folder)
{
	return 0;
}
