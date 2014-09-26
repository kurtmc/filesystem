/* 
 * Name: Kurt McAlpine
 * UPI: kmca733
 * ID: 2004750
 */
#include "fileutils.h"

#include <sys/types.h> /* for mkdir */
#include <sys/stat.h> /* for struct stat and function int stat (const char
		       *filename, struct stat *buf) */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h> /* for struct dirnet */

/* Function creates a new directory (if it does not already exist) with path current directory appended with
 * the directory "name", returns 0 on success
 */
int create_directory(char *name)
{
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

int create_file(char *name)
{
	FILE *fp = fopen(name, "ab+");
	return fclose(fp);
}

/* Function that iterates over the file names in a given directory */
struct dir_info {
	char *dir_path;
	DIR *real_dir;
};

struct dir_info *create_dir_info(char *dir_path_str)
{
	struct dir_info *inf = malloc(sizeof(struct dir_info));

	inf->dir_path = malloc(1024*sizeof(char));
	strcpy(inf->dir_path, dir_path_str);

	inf->real_dir = opendir(dir_path_str);

	return inf;
}

char *get_next_filename(struct dir_info *inf)
{
	struct dirent *next_file;

	char *file_path;
	file_path = malloc(1024 * sizeof(char));


	if (next_file = readdir(inf->real_dir)) {
		sprintf(file_path, "%s/%s", inf->dir_path, next_file->d_name);
		return next_file->d_name;
	}
	closedir(inf->real_dir);
	return NULL;
}

int compare (const void * a, const void * b)
{
	return strcmp(*(char**)a, *(char**)b);
}

char **get_all_filenames(char *dir_path_str)
{
	char **filenames = malloc(1024*sizeof(char *));
	int index = 0;

	struct dir_info *inf;
	inf = create_dir_info(dir_path_str);
	char *filename = malloc(1024 * sizeof(char));
	while (filename = get_next_filename(inf)) {
		/* Store all files names in list */
		filenames[index] = malloc(1024*sizeof(char));
		strcpy(filenames[index], filename);
		index++;
	}
	filenames[index] = NULL; /* Last element is null */
	qsort(filenames, index, sizeof(char*), compare);
	return filenames;
}
