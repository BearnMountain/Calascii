#include <stdlib.h>
#include <sys/ttycom.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <string.h>
#include <stdio.h>

#include "render.h"
#include "terminal.h"
#include "../../res/config.h"

struct ScreenBuffer screen_buffer = { NULL, 0 };
struct StatusBar status_bar = { NULL, 0, 0 };

void Render_init() {
	// init screen_buffer
	Render_drawRows();
	Render_screenBufferAppend("\x1b[H", 6); // sets cursor to top right
	
	// init status_bar
	status_bar.buffer_length = term.screen_cols;
	status_bar.buffer = malloc(status_bar.buffer_length * sizeof(char));
	for (int i = 0; i < status_bar.buffer_length; i++) status_bar.buffer[i] = ' ';

	Render_updateStatusBar();
}



// draws all the components
void Render_refreshScreen() {
	write(STDOUT_FILENO, "\x1b[2J\x1b[H", 7); // clear screen & 
	
	// loads buffer to screen
	write(STDOUT_FILENO, screen_buffer.buffer, screen_buffer.length);
	Render_updateStatusBar();
}

void Render_updateScreen(UpdateType u) {

	switch (u) {
		case UPDATED_CURSOR: {
			char cursor_buffer[16];
			snprintf(cursor_buffer, sizeof(cursor_buffer), "\x1b[%d;%dH", term.cursor_y, term.cursor_x);
			write(STDOUT_FILENO, cursor_buffer, strlen(cursor_buffer));
			break;
		}
		case UPDATED_STATUS_BAR:
			Render_updateStatusBar();
			break;
		default: 
			break;
	}
} 

void Render_updateStatusBar() {
	// moves cursor to the bottom of the screen
	char cursor_buffer[16];
	snprintf(cursor_buffer, sizeof(cursor_buffer), "\x1b[%d;1H", term.screen_rows + 1);
	write(STDOUT_FILENO, cursor_buffer, strlen(cursor_buffer));
	
	// set background
	write(STDOUT_FILENO, STATUS_BAR_BACKGROUND_COLOR, strlen(STATUS_BAR_BACKGROUND_COLOR));

	// print status_bar
	write(STDOUT_FILENO, status_bar.buffer, status_bar.buffer_length);

	// reset
	write(STDOUT_FILENO, "\x1b[0m", 4); // resets color
	snprintf(cursor_buffer, sizeof(cursor_buffer), "\x1b[%d;%dH", term.cursor_y, term.cursor_x);
	write(STDOUT_FILENO, cursor_buffer, strlen(cursor_buffer));
	fsync(STDOUT_FILENO);
}

void Render_drawRows() {
	for (int y = 0; y < term.screen_rows; y++) {
		Render_screenBufferAppend("~", 1);
		if (y < term.screen_rows - 1) {
			Render_screenBufferAppend("\r\n", 2);
		}
	}
}

void Render_screenBufferAppend(const char* string, int length) {
	char* new_buffer = realloc(screen_buffer.buffer, screen_buffer.length + length);

	if (new_buffer == NULL) return;
	memcpy(&new_buffer[screen_buffer.length], string, length); // copies in data to the very end of the buffer
	
	// updating variables
	screen_buffer.buffer = new_buffer;
	screen_buffer.length += length;
}

void Render_destructor() {
	free(screen_buffer.buffer); 
	free(status_bar.buffer);
}
