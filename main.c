#include <stdlib.h>
#include <string.h> /* for strcat */
#include <stdio.h>
#include <sys/types.h> /* for mkdir */
#include <sys/stat.h> /* for struct stat and function int stat (const char
		       *filename, struct stat *buf) */
#include <unistd.h> /* for getcwd */
int main(void)
{
	/* Initially check if the "A2dir" directory exists and if it doesn't
	 * create it */
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));
	printf("Current working directory: %s", cwd);

	char *a2dir;
	a2dir = malloc(strlen(cwd) + 6);
	a2dir[0] = '\0';
	strcat(a2dir, cwd);
	strcat(a2dir, "/A2dir");

	struct stat st = {0};

	if (stat(a2dir, &st) == -1) {
		    mkdir(a2dir, 0700);
	}

	char str[BUFSIZ];
	while (1) {
		printf("Enter a file name: ");
		fgets(str, BUFSIZ, stdin);
		printf("You entered: %s\n", str);
	}
	return 0;
}
