#include <stdio.h>
#include "serial.h"

int main(int argc, char** argv)
{
	if ( (argc < 2) || ((strcmp("/dev/ttyS0", argv[1])!=0) && (strcmp("/dev/ttyS4", argv[1])!=0) )) {
		printf("Usage:\tcommunicator SerialPort\n\tcentral: /dev/ttyS4\tTUX: /dev/ttyS0\n");
		exit(1);
	}

	unsigned char teste[] = {1, 2};
	printf("size: %d", sizeof(teste));

	int vtime = 30;
	int vmin = 0;
	int serial_fd = serial_initialize(argv[1], vmin, vtime);
	if(serial_fd < 0) exit(-1);
	
	//res = read(serial_fd, message, bytes);
	//res = write(serial_fd, message, strlen(message));

	serial_terminate(serial_fd);

	close(serial_fd);
	return 0;
}
