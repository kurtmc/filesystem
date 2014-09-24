#include "stringutils.h"
#include <string.h>

int compare_strings(char *str1, char *str2)
{
	if (strlen(str1) == strlen(str2)) {
		return strncmp(str1, str2, strlen(str1)) == 0;
	}
	return 0;
}
