#include <stdio.h>

int main(void)
{
	char str[BUFSIZ];
	while (1) {
		printf("Enter a file name: ");
		fgets(str, BUFSIZ, stdin);
		printf("You entered: %s\n", str);
	}
	return 0;
}
