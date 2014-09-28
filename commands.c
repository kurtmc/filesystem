/* 
 * Name: Kurt McAlpine
 * UPI: kmca733
 * ID: 2004750
 */
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

	dir = append_dash(dir);

	char **filenames = get_all_filenames(get_real_root_dir());
	int i = 0;
	while (filenames[i]) {
		if (filenames[i][0] != '.') {

			if (strncmp(dir, filenames[i], strlen(dir)) == 0 &&
					filenames[i][strlen(dir) - 1] == '-')
				return 1;
		}

		i++;
	}
	return 0;
}

char *get_absolute(char *fake_file) {
	if (fake_file[0] == '-')
		return fake_file;

	char *filename = malloc(1024*sizeof(char));
	strcpy(filename, get_fake_cwd());
	strcat(filename, fake_file);
	return filename;
}

char *get_real_filename(char *filename) {
	char *real_filename = malloc(1024*sizeof(char));
	strcpy(real_filename, get_real_root_dir());
	strcat(real_filename, "/");
	strcat(real_filename, get_absolute(filename));
	return real_filename;
}

/* Commands */

void pwd()
{
	printf("%s\n", get_fake_cwd());
}
void cd(char *args)
{
	if (args[0] == '\0') {
		set_fake_cwd("-");
	} else if (compare_strings(args, "..")) {
		char *cwd = get_fake_cwd();
		if (compare_strings(cwd, "-")) {
				return;
		} else {
			int i;
			int count = 0;
			for (i = strlen(cwd) - 1; i >= 0; i--) {
				if (cwd[i] == '-') {
					count++;
					if (count == 2) {
						cwd[i + 1] = '\0';
					}
				}
			}
		}
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
		cd(abs_path);
	}
}

void ls(char *fake_dir) {
	char *cwd;
	if (fake_dir[0] != '\0') {
		cwd = get_absolute(fake_dir);
		strcat(cwd, "-");
	} else {
		cwd = get_fake_cwd();
	}

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
	//system(rls_cmd);

	char file_type[40];

	FILE *fp = popen(rls_cmd, "r");
	if (fp == NULL) {
		printf("rls failed");
		exit -1;
	}

	while(fgets(file_type, sizeof(file_type), fp) != NULL) {
		printf("%s", file_type);
	}
	pclose(fp);
}
void tree(char *fake_dir)
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
void add(char *fake_file, char *str)
{
	/* get real filename */
	char *filename = get_real_filename(fake_file);
	//printf("real filename = %s\n", filename);
	//printf("last char = %d\n", filename[strlen(filename) - 1]);
	FILE *file = fopen(filename, "a");
	fprintf(file, "%s", str);
	fclose(file);
}
void cat(char *fake_file)
{
	/* get real filename */
	char *filename = get_real_filename(fake_file);
	FILE *file = fopen(filename, "r");
	char c;
	if (file) {
		while ((c = getc(file)) != EOF)
			putchar(c);
		fclose(file);
	}
	putchar('\n');
}
void delete(char *fake_file)
{
	fake_file = get_absolute(fake_file);
}
void dd(char *fake_dir)
{
	fake_dir = get_absolute(fake_dir);
}
void quit()
{
	exit(0);
}
