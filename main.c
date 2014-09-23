/* 
 * Name: Kurt McAlpine
 * UPI: kmca733
 * ID: 2004750
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "fileutils.h"

#define A2DIR "A2dir"

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

#define LSTR 1024

int compare_command(char *command, char *str)
{
	return strncmp(command, str, strlen(command)) == 0;
}

char *get_args(char *cmd_name, char *cmd)
{
	char *args = malloc(LSTR);
	strcpy(args, cmd);
	args = args + strlen(cmd_name) + 1;
	return args;
}


void execute_cmd(char *cmd) {
	if (cmd != NULL && strlen(cmd) > 0) {
		cmd[strlen(cmd) - 1] = '\0';
	} else {
		return;
	}

	/* Execute corresponding command */
	if (compare_command(PWD, cmd)) {
		pwd();
	} else if (compare_command(CD, cmd)) {
		cd();
	} else if (compare_command(LS, cmd)) {
		ls();
	} else if (compare_command(RLS, cmd)) {
		rls();
	} else if (compare_command(TREE, cmd)) {
		tree();
	} else if (compare_command(CLEAR, cmd)) {
		clear();
	} else if (compare_command(CREATE, cmd)) {
		char *filename = get_args(CREATE, cmd);
		create(filename);
	} else if (compare_command(ADD, cmd)) {
		add();
	} else if (compare_command(CAT, cmd)) {
		cat();
	} else if (compare_command(DELETE, cmd)) {
		delete();
	} else if (compare_command(DD, cmd)) {
		dd();
	} else if (compare_command(QUIT, cmd)) {
		quit();
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
