#include <stdio.h>
#include <string.h>
#include "datalink.h"

int main(int argc, char** argv)
{
	if(argc != 2) {
		printf("INVALID ARGS");
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

	if(strcmp(argv[1], "TRANSMITTER") == 0) {
		/*int fd = */llopen("/dev/ttyS0", TRANSMITTER);
	} else if(strcmp(argv[1], "RECEIVER") == 0) {
		/*int fd = */llopen("/dev/ttyS0", RECEIVER);
	}

	return 0;
}
