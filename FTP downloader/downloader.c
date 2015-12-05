#include <regex.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define FTP_DEFAULT_PORT 22

bool validateURL(const char *url);
int parseURL(const char *url, char **user, char **pass, char **host, char **dir);
int socket_connect(struct in_addr *server_address, unsigned server_port);
int host_to_address(const char *host, struct in_addr *address);

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("Usage: %s <url>\n", argv[0]);
		return 1;
	}
	if (!validateURL(argv[1])) return 1;

	char *user;
	char *pass;
	char *host;
	char *dir;
	parseURL(argv[1], &user, &pass, &host, &dir);

	struct in_addr address;
	if (host_to_address(host, &address)) return 1;
	int sockfd = socket_connect(&address, FTP_DEFAULT_PORT);
	if (sockfd < 0) return 1;

	free(user);
	free(pass);
	free(host);
	free(dir);

	return 0;
}

bool validateURL(const char *url)
{
	char regexString[] = "^ftp://"	// ftp://
			"("
			"("
			"("
			"[^:@/]*"	// <user>
			")"
			"("
			":"			// :
			"("
			"[^:@/]*"	// <pass>
			")"
			")?"
			")?"
			"@"			// @
			")?"
			"("
			"[^:@/]+"	// <host>
			")"
			"/"			// /
			"(.*)"		// <dir>
			"$";

	regex_t regex;

	int res;
	if ((res = regcomp(&regex, regexString, REG_EXTENDED)))
	{
		printf("Error #%d compiling regex.\n", res);
		return false;
	}
	else
		printf("Regex compiled successfully.\n");

	if (regexec(&regex, url, 0, NULL, 0) == 0)
	{
		printf("Regex validated.\n");
		regfree(&regex);
		return true;
	}
	else
	{
		printf("Regex validation failed.\n");
		regfree(&regex);
		return false;
	}
}

int parseURL(const char *url, char **user, char **pass, char **host, char **dir)
{
	const char *temp = url;
	if ((temp = strchr(temp, '/')) == NULL) return 1;
	if ((temp = strchr(++temp, '/')) == NULL) return 1;

	unsigned length;
	char *atSign = strchr(++temp, '@');
	if (atSign == NULL)
	{
		*user = NULL;
		*pass = NULL;
	}
	else
	{
		char *colon = strchr(temp, ':');
		length = (colon == NULL ? atSign : colon) - temp;
		if ((*user = malloc((length + 1) * sizeof(char))) == NULL) return 1;
		memcpy(*user, temp, length);
		*(*user + length) = '\0';

		if ((colon != NULL && colon < atSign)) // Has password
		{
			temp = colon + 1;
			length = atSign - temp;
			if ((*pass = malloc((length + 1) * sizeof(char))) == NULL) return 1;
			memcpy(*pass, temp, length);
			*(*pass + length) = '\0';
		}
		else *pass = NULL;
	}

	temp = (atSign == NULL ? temp : atSign + 1);
	char *dir_slash = strchr(temp, '/');
	length = dir_slash - temp;
	if ((*host = malloc((length + 1) * sizeof(char))) == NULL) return 1;
	memcpy(*host, temp, length);
	*(*host + length) = '\0';

	temp = dir_slash + 1;
	length = strlen(temp);
	if ((*dir = malloc((length + 1) * sizeof(char))) == NULL) return 1;
	memcpy(*dir, temp, length);
	*(*dir + length) = '\0';

	if (*user != NULL) printf("User: %s\n", *user);
	if (*pass != NULL) printf("Pass: %s\n", *pass);
	if (*host != NULL) printf("Host: %s\n", *host);
	if (*dir != NULL) printf("Dir: %s\n", *dir);

	return 0;
}

int socket_connect(struct in_addr *server_address, unsigned server_port) {
	int	sockfd;
	struct	sockaddr_in server_addr;
	char	buf[] = "Mensagem de teste na travessia da pilha TCP/IP\n";
	int	bytes;

	/*server address handling*/
	bzero((char*)&server_addr,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = server_address->s_addr;	/*32 bit Internet address network byte ordered*/
	server_addr.sin_port = htons(server_port);		/*server TCP port must be network byte ordered */
	printf("IP: %s\n", inet_ntoa(server_addr.sin_addr));
	/*open an TCP socket*/
	if ((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0) {
		perror("socket()");
		return -1;
	}
	/*connect to the server*/
	if(connect(sockfd,
			(struct sockaddr *)&server_addr,
			sizeof(server_addr)) < 0){
		perror("connect()");
		return -1;
	}

	return sockfd;
}

int host_to_address(const char *host, struct in_addr *address) {
	struct hostent *h;
	if ((h=gethostbyname(host)) == NULL) {
		herror("gethostbyname");
		return 1;
	}
	address = (struct in_addr *)h->h_addr;
	return 0;
}
