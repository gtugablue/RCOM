#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "datalink.h"

int main(int argc, char** argv)
{
	if (argc != 4)
		{
			printf("Usage: %s <port> <send|receive> <filename>\n", argv[0]);
			return 1;
		}
	/*if ( (argc < 2) || ((strcmp("/dev/ttyS0", argv[1])!=0) && (strcmp("/dev/ttyS4", argv[1])!=0) )) {
		printf("Usage:\tcommunicator SerialPort\n\tcentral: /dev/ttyS4\tTUX: /dev/ttyS0\n");
		exit(1);
	}*/

	/*int vtime = 30;
	int vmin = 0;
	int serial_fd = serial_initialize(argv[1], vmin, vtime);
	if(serial_fd < 0) exit(-1);*/
	
	//res = read(serial_fd, message, bytes);
	//res = write(serial_fd, message, strlen(message));

	//serial_terminate(serial_fd);

	if(strcmp(argv[2], "SENDER") == 0) {
		int fd = llopen(argv[1], SENDER);
		sleep(1);
		llclose(fd, SENDER);
	} else if(strcmp(argv[2], "RECEIVER") == 0) {
		int fd = llopen(argv[1], RECEIVER);
		sleep(1);
		llclose(fd, RECEIVER);
	}

	return 0;
}
