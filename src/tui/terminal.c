#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>

#include "terminal.h"
#include "../debug/error.h"

struct TerminalConfig term; // intializing term

void Terminal_disableRawMode() {
	// resets terminal to original state
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &term.original_terminal) == -1)
		die("tcsetattr");
}

void Terminal_enableRawMode() {
	// grabbing terminal values and saving them
	if (tcgetattr(STDIN_FILENO, &term.original_terminal) == -1)
		die("tcgetattr");
	atexit(Terminal_disableRawMode); // cleanup

	// applies changes to terminal buffer
	struct termios raw = term.original_terminal;
	raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	raw.c_oflag &= ~(OPOST);
	raw.c_cflag |= (CS8);
	raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	raw.c_cc[VMIN] = 0;
	raw.c_cc[VTIME] = 1;

	// loads buffer into terminal 	
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
		die("tcsetattr");
}

void Terminal_init() {
	if (Terminal_getWindowSize(&term.screen_rows, &term.screen_cols) == -1)
		die("getWindowSize");

	term.screen_rows--;
	term.cursor_x = 0;
	term.cursor_y = 0;
}

int Terminal_getWindowSize(int* rows, int* cols) {
	struct winsize win;

	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &win) == -1 || win.ws_col == 0) {
		if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12) return -1;
		return Terminal_getCursorPosition(rows, cols);
	} else {
		*cols = win.ws_col;
		*rows = win.ws_row;
		return 0;
	}
}

int Terminal_getCursorPosition(int* rows, int* cols) {
	char buffer[32];
	unsigned int i = 0;

	if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4)
		return -1;

	while (i < sizeof(buffer) - 1) {
		if (read(STDIN_FILENO, &buffer[i], 1) != 1) break;
		if (buffer[i] == 'R') break;

		i++;
	}
	buffer[i] = '\0';

	// reads buffered cursor pos
	if (buffer[0] != '\x1b' || buffer[1] != '[') return -1;
	if (sscanf(&buffer[2], "%d;%d", rows, cols) != 2) return -1;

	return -1;
}

void Terminal_setCursorPosition(int x, int y) {
	char cursor_buffer[16];
	snprintf(cursor_buffer, sizeof(cursor_buffer), "\x1b[%d;%dH", y, x);
	write(STDOUT_FILENO, cursor_buffer, strlen(cursor_buffer));

	term.cursor_x = x;
	term.cursor_y = y;
}

