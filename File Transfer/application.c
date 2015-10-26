#include "application.h"
#include "datalink.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <libgen.h>

#define MAX(A, B) (((A) > (B)) ? (A) : (B))
#define MIN(A, B) (((A) < (B)) ? (A) : (B))
#define GET_BYTE(X, N) (((X) & (0xFF << (N * 8))) >> (N * 8))

typedef enum {
	PACKET_CTRL_TYPE_SIZE,
	PACKET_CTRL_TYPE_NAME
} packet_ctrl_type_t;

typedef enum {
	PACKET_CTRL_FIELD_DATA = 0,
	PACKET_CTRL_FIELD_START = 1,
	PACKET_CTRL_FIELD_END = 2
} packet_ctrl_field_t;

typedef struct {
	packet_ctrl_type_t type;
	char length;
	char *value;
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
int send_control_packet(datalink_t *datalink, const control_packet_t *control_packet);

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
		if (send_file(argv[1], argv[3]))
		{
			printf("Error sending file.\n");
			return 1;
		}
		else
			printf("File sent successfully.\n");
	} else if(strcmp(argv[2], "receive") == 0) {
		datalink_init(&datalink, RECEIVER);
		llopen(argv[1], &datalink);
		char buf[1000000];
		printf("Reading...\n");
		llread(&datalink, buf);
		printf("Read done.\n");
		// TODO
	}
	return 0;
}

int send_file(const char *port, const char *file_name)
{
	// Read file
	FILE *fp = fopen(file_name, "r");
	fseek(fp, 0, SEEK_END);
	unsigned long size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	unsigned char data[size];
	fread(data, size, 1, fp);
	fclose(fp);

	// Establish connection
	datalink_t datalink;
	datalink.mode = SENDER;
	if (llopen(port, &datalink)) return 1;

	// Send start packet
	control_packet_t control_packet;
	control_packet.ctrl_field = PACKET_CTRL_FIELD_START;
	control_packet.num_params = 2;

	control_packet_param_t param_size;
	param_size.type = PACKET_CTRL_TYPE_SIZE;
	char str[(int)((ceil(log10(size)) + 1) * sizeof(char))];
	if (sprintf(str, "%lu", size) < 0) return 1;
	param_size.length = strlen(str);
	param_size.value = str;

	control_packet_param_t param_name;
	param_name.type = PACKET_CTRL_TYPE_NAME;
	char copy[strlen(file_name) + 1];
	strcpy(copy, file_name);
	char *base = basename(copy);
	char file[strlen(base) + 1];
	strcpy(file, base);
	param_name.length = strlen(file);
	param_name.value = file;

	control_packet_param_t params[] = {param_size, param_name};
	control_packet.params = params;
	if (send_control_packet(&datalink, &control_packet)) return 1;

	// Send data packet
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

	// Send end packet
	control_packet.ctrl_field = PACKET_CTRL_FIELD_END;
	if (send_control_packet(&datalink, &control_packet)) return 1;

	return llclose(&datalink);
}

int send_control_packet(datalink_t *datalink, const control_packet_t *control_packet)
{
	printf("Sending control packet...\n");
	unsigned size = 4;
	unsigned i;
	for (i = 0; i < control_packet->num_params; ++i)
	{
		size += control_packet->params[i].length;
	}
	unsigned char packet[size];
	packet[0] = control_packet->ctrl_field;
	unsigned j;
	for (i = 0, j = 1; i < control_packet->num_params; ++i)
	{
		packet[j++] = control_packet->params[i].type;
		packet[j++] = control_packet->params[i].length;
		memcpy(&packet[j], control_packet->params[i].value, control_packet->params[i].length);
		j += control_packet->params[i].length;
	}
	if (llwrite(datalink, packet, size))
	{
		printf("Error sending control packet.\n");
		return 1;
	}
	printf("Sent control packet with size %d.\n", size);
	return 0;
}

int send_data_packet(datalink_t *datalink, const data_packet_t *data_packet)
{
	printf("Sending data packet...\n");
	unsigned size = data_packet->length + 4;
	unsigned char packet[size];
	packet[0] = data_packet->ctrl_field;
	packet[1] = data_packet->sn;
	packet[2] = (uint8_t)((data_packet->length & 0xFF00) >> 8);
	packet[3] = (uint8_t)(data_packet->length & 0x00FF);
	memcpy(&packet[4], data_packet->data, size);
	if (llwrite(datalink, packet, size))
	{
		printf("Error data control packet.\n");
		return 1;
	}
	printf("Sent data packet with size %d.\n", size);
	return 0;
}

int receive_file(const char *port, const char *destination_folder)
{

	return 0;
}
