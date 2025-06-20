#ifndef TERMINAL_H
#define TERMINAL_H

#include <termios.h>

struct TerminalConfig {
	int cursor_x, cursor_y; // should only be in the textbox area
	int screen_rows, screen_cols;
	struct termios original_terminal;
};

extern struct TerminalConfig term;

void Terminal_init();

// turning off terminal buffers for program buffers
void Terminal_disableRawMode();
void Terminal_enableRawMode();

// SETTERS & GETTERS for terminal states
int Terminal_getWindowSize(int* rows, int* cols);
int Terminal_getCursorPosition(int* rows, int* cols);
void Terminal_setCursorPosition(int x, int y);

#endif
