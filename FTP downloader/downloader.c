#include <regex.h>
#include <stdio.h>
#include <string.h>

int main()
{
	char source[] = "ftp://fe.up.pt/dir";
	char regexString[] = "^ftp://((([^:@/]*)(:([^:@/]*))?)?@)?([^:@/]+)/(.*)$";

	regex_t regex;

	size_t maxGroups = 10;
	regmatch_t groupArray[maxGroups];

	int res;
	if ((res = regcomp(&regex, regexString, REG_EXTENDED)))
	{
		printf("Error #%d compiling regex.\n", res);
		return 1;
	}
	else
		printf("Regex compiled successfully.\n");

	if (regexec(&regex, source, maxGroups, groupArray, 0) == 0)
	{
		printf("Regex validated.\n");

		unsigned int g = 0;
		for (g = 0; g < maxGroups; g++)
		{
			if (groupArray[g].rm_so == (size_t)-1)
				break; // No more groups

			char sourceCopy[strlen(source) + 1];
			strcpy(sourceCopy, source);
			sourceCopy[groupArray[g].rm_eo] = 0;
			printf("Group %u: [%2u-%2u]: %s\n",
					g, groupArray[g].rm_so, groupArray[g].rm_eo,
					sourceCopy + groupArray[g].rm_so);
		}
	}
	else
		printf("Regex validation failed.\n");

	regfree(&regex);

	return 0;
}
