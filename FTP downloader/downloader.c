#include <regex.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

bool validateURL(const char *url);
int parseURL(const char *url, char **user, char **pass, char **host, char **dir);

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
	free(user);
	free(pass);
	free(host);
	free(dir);

	return 0;
}

bool validateURL(const char *url)
{
	char regexString[] = "^ftp://"		// ftp://
			"("
			"("
			"("
			"[^:@/]*"		// <user>
			")"
			"("
			":"				// :
			"("
			"[^:@/]*"	// <pass>
			")"
			")?"
			")?"
			"@"						// @
			")?"
			"("
			"[^:@/]+"				// <host>
			")"
			"/"							// /
			"(.*)"						// <dir>
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
