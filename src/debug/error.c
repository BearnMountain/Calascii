#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "error.h"

void die(const char* s) {
	write(STDOUT_FILENO, "\x1b[2J", 4); // clear screen
	write(STDOUT_FILENO, "\x1b[H", 3); // set cursor to top left 

	perror(s);
	exit(1);
}
