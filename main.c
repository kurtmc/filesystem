/* 
 * Name: Kurt McAlpine
 * UPI: kmca733
 * ID: 2004750
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h> /* for struct dirnet */

#include "fileutils.h"

#define A2DIR "A2dir"

#define FILELEN 256

#define PWD "pwd"
#define CD "cd"
#define LS "ls"
#define RLS "rls"
#define TREE "tree"
#define CLEAR "clear"
#define CREATE "create"
#define ADD "add"
#define CAT "cat"
#define DELETE "delete"
#define DD "dd"
#define QUIT "quit"

static char *fake_cwd;

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

static char *get_fake_cwd()
{
	if (fake_cwd == NULL) {
		fake_cwd = malloc(FILELEN * sizeof(char));
		fake_cwd[0] = '\0';
		strcpy(fake_cwd, "-");
	}
	return fake_cwd;
}

int clear_real_dir()
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
	return 0;
}

void ls() {
	struct dirent *next_file;

	DIR *real_dir;

	char *file_path;
	file_path = malloc(FILELEN * sizeof(char));

	real_dir = opendir(get_real_root_dir());

	while (next_file = readdir(real_dir)) {
		sprintf(file_path, "%s/%s", get_real_root_dir(),
				next_file->d_name);
		if (next_file->d_name[0] != '.')
			if (strncmp(get_fake_cwd(), next_file->d_name, strlen(get_fake_cwd())) == 0)
				printf("%s\n",
						&next_file->d_name[strlen(get_fake_cwd())]);
	}
	closedir(real_dir);

}

int compare_command(char *command, char *str)
{
	return strncmp(command, str, strlen(command)) == 0;
}

void execute_cmd(char *cmd) {
	if (cmd != NULL && strlen(cmd) > 0) {
		cmd[strlen(cmd) - 1] = '\0';
	} else {
		return;
	}
	if (compare_command(CREATE, cmd)) {
		/* Remove the command from the front of the string */
		char *filename = cmd + strlen(CREATE) + 1;
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
	} else if (compare_command(CLEAR, cmd)) {
		clear_real_dir();
	} else if (compare_command(PWD, cmd)) {
		printf("%s\n", get_fake_cwd());
	} else if (compare_command(RLS, cmd)) {
		char *rls_cmd;
		rls_cmd = malloc(FILELEN * sizeof(char));
		rls_cmd[0] = '\0';
		strcpy(rls_cmd, "ls -l ");
		strcat(rls_cmd, get_real_root_dir());
		system(rls_cmd);
	} else if (compare_command(QUIT, cmd)) {
		exit(0);
	} else if (compare_command(LS, cmd)) {
		ls();
	}

}

int main(void)
{
	/* Initially check if the "A2dir" directory exists and if it doesn't
	 * create it */
	create_directory(A2DIR);
	char str[BUFSIZ];
	if (isatty(fileno(stdin))) {
		printf("\nffs> ");
		while (fgets(str, BUFSIZ, stdin) != NULL) {
			//printf("You entered: %s", str);
			execute_cmd(str);
			printf("ffs> ");
		}
	} else {
		printf("\n");
		while (fgets(str, BUFSIZ, stdin) != NULL) {
			printf("ffs> %s", str);
			execute_cmd(str);
		}
	}
	return 0;
}
