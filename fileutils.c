#include "fileutils.h"

#include <sys/types.h> /* for mkdir */
#include <sys/stat.h> /* for struct stat and function int stat (const char
		       *filename, struct stat *buf) */
#include <stdlib.h>
#include <string.h>

/* Function creates a new directory (if it does not already exist) with path current directory appended with
 * the directory "name", returns 0 on success
 */
int create_directory(char *name) {
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));

	char *full_path;
	full_path = malloc(strlen(cwd) + 6);
	full_path[0] = '\0';
	strcat(full_path, cwd);
	strcat(full_path, "/");
	strcat(full_path, name);

	struct stat st = {0};

	if (stat(full_path, &st) == -1) {
		    return mkdir(full_path, 0700);
	}

	return 0;
}
