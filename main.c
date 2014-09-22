/* 
 * Name: Kurt McAlpine
 * UPI: kmca733
 * ID: 2004750
 */

#include <stdlib.h>
#include <stdio.h>

#include "fileutils.h"



int main(void)
{
	/* Initially check if the "A2dir" directory exists and if it doesn't
	 * create it */
	create_directory("A2dir");

	char str[BUFSIZ];
	if (isatty(fileno(stdin))) {
		printf("\nffs> ");
		while (fgets(str, BUFSIZ, stdin) != NULL) {
			printf("You entered: %s", str);
			printf("ffs> ");
		}
	} else {
		printf("\n");
		while (fgets(str, BUFSIZ, stdin) != NULL) {
			printf("ffs> %s", str);
			printf("You entered: %s", str);
		}
	}
	return 0;
}
