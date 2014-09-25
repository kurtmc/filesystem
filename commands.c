#include "commands.h"
#include "fileutils.h"
#include "stringutils.h"

#include <stdlib.h>
#include <stdio.h>
#include <dirent.h> /* for struct dirnet */
#include <string.h>

#define A2DIR "A2dir"
#define FILELEN 256

/* Helper methods */
static char *fake_cwd;
static char *get_fake_cwd()
{
	if (fake_cwd == NULL) {
		fake_cwd = malloc(FILELEN * sizeof(char));
		fake_cwd[0] = '\0';
		strcpy(fake_cwd, "-");
	}
	return fake_cwd;
}
static void set_fake_cwd(char *wd)
{
	if (fake_cwd == NULL) {
		fake_cwd = malloc(FILELEN * sizeof(char));
		fake_cwd[0] = '\0';
	}
	strcpy(fake_cwd, wd);
}


char *get_real_root_dir() {
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));

	char *full_path;
	full_path = malloc(FILELEN * sizeof(char));
	full_path[0] = '\0';
	strcat(full_path, cwd);
	strcat(full_path, "/");
	strcat(full_path, A2DIR);

	return full_path;
}

char *append_dash(char *path)
{
	if (path[strlen(path) - 1] != '-') {
		char* fixed_dir = malloc(1024*sizeof(char));
		strncpy(fixed_dir, path, strlen(path));
		fixed_dir[strlen(path)] = '-';
		fixed_dir[strlen(path) + 1] = '\0';
		return fixed_dir;
	}
	return path;
}

int directory_exists(char *dir)
{

	/* if it doesn't end in '-', append it */
	/*if (dir[strlen(dir) - 1] != '-') {
		char* fixed_dir = malloc(1024*sizeof(char));
		strncpy(fixed_dir, dir, strlen(dir));
		fixed_dir[strlen(dir)] = '-';
		fixed_dir[strlen(dir) + 1] = '\0';
		dir = fixed_dir;

	}*/
	dir = append_dash(dir);

	char **filenames = get_all_filenames(get_real_root_dir());
	int i = 0;
	while (filenames[i]) {
		if (filenames[i][0] != '.') {
			//printf("dir = %s\n", dir);
			//printf("filenames[%d] = %s\n", i, filenames[i]);

			if (strncmp(dir, filenames[i], strlen(dir)) == 0 &&
					filenames[i][strlen(dir) - 1] == '-')
				return 1;
		}

		i++;
	}
	return 0;
}
/* Commands */

void pwd()
{
	printf("%s\n", get_fake_cwd());
}
void cd(char *args)
{
	//printf("cd called args = |%s|\n", args);
	if (args[0] == '\0') {
		set_fake_cwd("-");
	} else if (args[0] == '-') { /* Absolute */
		/* Need to check that directory actually exists first */
		if (directory_exists(args)) {
			args = append_dash(args);
			set_fake_cwd(args);
		} else {
			printf("Directory does not exist\n");
		}
	} else { /* Relative */
		/* Convert to absolute and call recursively */
		char *abs_path = malloc(1024);
		strcpy(abs_path, get_fake_cwd());
		strcat(abs_path, args);
		//printf("abs_path = %s\n", abs_path);
		cd(abs_path);
	}
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

void ls() {
	char *cwd = get_fake_cwd();

	char *last_dir_printed = malloc(1024 * sizeof(char));
	last_dir_printed[0] = '\0';
	char *to_print = malloc(1024 * sizeof(char));
	to_print[0] = '\0';

	char **filenames = get_all_filenames(get_real_root_dir());
	int i = 0;
	while (filenames[i]) {
		if (filenames[i][0] != '.' && strncmp(filenames[i], cwd, strlen(cwd)) == 0) {
			strcpy(to_print, &filenames[i][strlen(cwd)]);
			int dash_pos = get_pos_of_nth_occurance_of(to_print, '-', 1);
			if (dash_pos > 0) {
				to_print[dash_pos] = '\0';
				if (!compare_strings(last_dir_printed, to_print)) {
					printf("d: %s\n", to_print);
					strcpy(last_dir_printed, to_print);
				}
			} else {
				printf("f: %s\n", to_print);
			}
		}
		i++;
	}

}
void rls()
{
	char *rls_cmd;
	rls_cmd = malloc(FILELEN * sizeof(char));
	rls_cmd[0] = '\0';
	strcpy(rls_cmd, "ls -l ");
	strcat(rls_cmd, get_real_root_dir());
	system(rls_cmd);
}
void tree()
{
}
void clear()
{
	struct dirent *next_file;

	DIR *real_dir;

	char *file_path;
	file_path = malloc(FILELEN * sizeof(char));

	real_dir = opendir(get_real_root_dir());

	while (next_file = readdir(real_dir)) {
		sprintf(file_path, "%s/%s", get_real_root_dir(),
				next_file->d_name);
		if (next_file->d_name[0] != '.')
			remove(file_path);
	}
	closedir(real_dir);
}
void create(char *filename)
{
	if (filename[0] == '-') { /* Absolute */
		char *full_name = malloc(256 * sizeof(char));
		full_name[0] = '\0';
		strcpy(full_name, get_real_root_dir());
		strcat(full_name, "/");
		strcat(full_name, filename);
		create_file(full_name);
	} else { /* Relative */
		char *full_name = malloc(256 * sizeof(char));
		full_name[0] = '\0';
		strcpy(full_name, get_real_root_dir());
		strcat(full_name, "/");
		strcat(full_name, get_fake_cwd());
		strcat(full_name, filename);
		create_file(full_name);
	}
}
void add()
{
}
void cat()
{
}
void delete()
{
}
void dd()
{
}
void quit()
{
	exit(0);
}
