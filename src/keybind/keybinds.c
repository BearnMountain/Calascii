#include <errno.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "keybinds.h"
#include "../tui/terminal.h"
#include "../tui/render.h"
#include "../debug/error.h"

#include "../../res/config.h"

#define CTRL_KEY(k) ((k) & 0x1f)

// defining return values above 7-bit ascii
#define NEW_LINE 128
#define TAB 129
#define SHIFT 130

char Keybind_readKey() {
	int input;
	char c;

	// gets first input character
	while ((input = read(STDIN_FILENO, &c, 1)) != 1) {
		if (input == -1 && errno != EAGAIN) die("read");
	}

	// reads special sequences
	if (c == '\x1b') {
		char sequence[3];

		if (read(STDIN_FILENO, &sequence[0], 1) != 1 && read(STDIN_FILENO, &sequence[1], 1) != 1) return '\x1b';

		// arrow keys
		if (sequence[0] == '[') {
			switch (sequence[1]) {
				case 'A': return MOVE_UP;
				case 'B': return MOVE_DOWN;
				case 'C': return MOVE_RIGHT;
				case 'D': return MOVE_LEFT;
			}
		}

		return '\x1b';
	}

	return c;
}

void Keybind_processKeypress() {
	char c = Keybind_readKey();

	// status_bar always intercepts keys
	Keybind_updateStatusBar(c);
	
	switch (c) {
		case CTRL_KEY('q'):
			write(STDOUT_FILENO, "\x1b[2J", 4);
			write(STDOUT_FILENO, "\x1b[H", 3);
			Render_destructor();
			Terminal_disableRawMode();
			exit(0);
			break;
		case CTRL_KEY('c'):
			write(STDOUT_FILENO, "\x1b[2J", 4);
			write(STDOUT_FILENO, "\x1b[H", 3);
			Render_destructor();
			Terminal_disableRawMode();
			exit(0);
			break;
		case MOVE_UP:
		case MOVE_LEFT:
		case MOVE_DOWN:
		case MOVE_RIGHT:
			Keybind_moveCursor(c);
			
			break;
		default:
			Render_updateScreen(UPDATE_TEXT);
			break;
	}
}

void Keybind_updateStatusBar(char key) {

	if (status_bar.active != 1) {
		if (key == ':') {
			status_bar.active = 1;
			status_bar.buffer[status_bar.string_length] = key;
			status_bar.string_length++;
			Render_updateScreen(UPDATE_STATUS_BAR);
		}
		return;
	} 

	// adds characters
	if (key > 31 && key < 127) {
		status_bar.buffer[status_bar.string_length] = key;
		status_bar.string_length++;
	} else if (key == 127 && status_bar.string_length > 0) {
		status_bar.buffer[status_bar.string_length - 1] = ' ';
		status_bar.string_length--;
	}


	// sets
	if (key == '\n' || key == '\r') {
		// executing the buffer commands


		status_bar.active = 0;
		for (uint16_t i = 0; i < status_bar.string_length; i++) 
			status_bar.buffer[i] = ' ';
		status_bar.string_length = 0;
	}
	// escapes
	if (key == '\x1b') {
		status_bar.active = 0;
		for (uint16_t i = 0; i < status_bar.string_length; i++) 
			status_bar.buffer[i] = ' ';
		status_bar.string_length = 0;
	}
	Render_updateScreen(UPDATE_STATUS_BAR);
}

void Keybind_moveCursor(char key) {
	switch (key) {
		case MOVE_UP:
			if (term.cursor_y == 0) return;
			term.cursor_y--;
			break;
		case MOVE_LEFT:
			if (term.cursor_x == 0) return;
			term.cursor_x--;
			break;
		case MOVE_DOWN:
			if (term.cursor_y == term.screen_rows) return;
			term.cursor_y++;
			break;
		case MOVE_RIGHT:
			if (term.cursor_x == term.screen_cols) return;
			term.cursor_x++;
			break;
		default:
			break;
	}

	Render_updateScreen(UPDATE_CURSOR);
}
