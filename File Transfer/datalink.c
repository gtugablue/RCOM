#include "datalink.c"

int llopen(int porta, int mode) {

}

int llwrite(int fd, char * buffer, int length) {

}

int llread(int fd, char * buffer) {

}

int llclose(int fd) {

}

char* get_packet(int fd) {
	state_t state = START;
	unsigned char byte;
	packet_t packet;

	while(state != STOP) {
		int ret = read_byte(fd, &byte);
		if(ret != 0) {
			perror("ERROR READING FROM SERIAL PORT\n");
			exit(-1);
		}

		switch(state) {
		case START:

			break;
		case STOP:
			break;
		}
	}
}
