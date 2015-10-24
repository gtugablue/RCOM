#include "application.h"

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

}

int receive_file(const unsigned char *port, const unsigned char *destination_folder)
{

}
