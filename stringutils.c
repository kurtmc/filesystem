#include "stringutils.h"
#include <string.h>

int compare_strings(char *str1, char *str2)
{
	if (strlen(str1) == strlen(str2)) {
		return strncmp(str1, str2, strlen(str1)) == 0;
	}
	return 0;
}

/* Returns -1 if not found */
int get_pos_of_nth_occurance_of(char* str, char c, int n)
{
	int i;
	int count = 0;
	for (i = 0; str[i] != '\0'; i++) {
		if (str[i] == c) {
			count++;
			if (count == n) {
				return i;
			}
		}
	}
	return -1;
}
