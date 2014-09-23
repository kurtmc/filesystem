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
		create(filename);
	} else if (compare_command(CLEAR, cmd)) {
		clear();
	} else if (compare_command(PWD, cmd)) {
		pwd();
	} else if (compare_command(RLS, cmd)) {
		rls();
	} else if (compare_command(QUIT, cmd)) {
		quit();
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
