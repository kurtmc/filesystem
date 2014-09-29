/* 
 * Name: Kurt McAlpine
 * UPI: kmca733
 * ID: 2004750
 *
 * Implementing this in c was a terrible idea, do not recommend
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "fileutils.h"
#include "stringutils.h"

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
	return strncmp(command, str, strlen(command)) == 0 &&
		(str[strlen(command)] == ' ' || str[strlen(command)] == '\0' ||
		 str[strlen(command)] == '\n');
}

char *get_args(char *cmd_name, char *cmd)
{
	char *args = malloc(LSTR);
	strcpy(args, cmd + strlen(cmd_name) + 1);
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
		cd(get_args(CD, cmd));
	} else if (compare_command(LS, cmd)) {
		char *filename = get_args(LS, cmd);
		ls(filename);
	} else if (compare_command(RLS, cmd)) {
		rls();
	} else if (compare_command(TREE, cmd)) {
		//printf("Tree cmd\n");
		char *filename = get_args(TREE, cmd);
		tree(filename, 0);
	} else if (compare_command(CLEAR, cmd)) {
		clear();
	} else if (compare_command(CREATE, cmd)) {
		char *filename = get_args(CREATE, cmd);
		create(filename);
	} else if (compare_command(ADD, cmd)) {
		char *args = get_args(ADD, cmd);
		int space_1_pos = get_pos_of_nth_occurance_of(args, ' ', 1);

		char *filename = malloc(1024*sizeof(char));
		strncpy(filename, args, space_1_pos);
		filename[strlen(filename) - 1] = '\0';
		//printf("main.c filename last char = %d\n",
				//filename[strlen(filename) - 1]);
		char *string_to_add = malloc(1024*sizeof(char));
		strcpy(string_to_add, &args[space_1_pos + 1]);

		add(filename, string_to_add);
	} else if (compare_command(CAT, cmd)) {
		char *filename = get_args(CAT, cmd);
		cat(filename);
	} else if (compare_command(DELETE, cmd)) {
		char *filename = get_args(DELETE, cmd);
		delete(filename);
	} else if (compare_command(DD, cmd)) {
		char *filename = get_args(DD, cmd);
		dd(filename);
	} else if (compare_command(QUIT, cmd)) {
		quit();
	} else {
		if (isatty()) {
			printf("Command not found\n");
		}
	}
}

int main(void)
{
	/* Initially check if the "A2dir" directory exists and if it doesn't
	 * create it */
	create_directory(A2DIR);
	char str[BUFSIZ];
	if (isatty(fileno(stdin))) {
		printf("ffs> ");
		while (fgets(str, BUFSIZ, stdin) != NULL) {
			//printf("You entered: %s", str);
			execute_cmd(str);
			printf("ffs> ");
		}
	} else {
		while (fgets(str, BUFSIZ, stdin) != NULL) {
			printf("ffs> %s", str);
			execute_cmd(str);
		}
	}
	return 0;
}
