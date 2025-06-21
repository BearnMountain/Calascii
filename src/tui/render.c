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

struct ScreenBuffer screen_buffer = { NULL, 0, 0 };
struct StatusBar status_bar = { 0, NULL, 0, 0 };

void Render_init() {
	// init screen_buffer
	// Render_screenBufferAppend("\x1b[H", 6); // sets cursor to top right
	write(STDIN_FILENO, "\x1b[2J", 4);
	write(STDIN_FILENO, "\x1b[H", 3);
	
	// init status_bar
	status_bar.buffer_length = term.screen_cols;
	status_bar.buffer = malloc(status_bar.buffer_length * sizeof(char));
	for (int i = 0; i < status_bar.buffer_length; i++) status_bar.buffer[i] = ' ';

	Render_updateStatusBar();
}



// draws all the components
void Render_updateScreen(UpdateType u) {

	switch (u) {
		case UPDATE_CURSOR: {
			char cursor_buffer[16];
			snprintf(cursor_buffer, sizeof(cursor_buffer), "\x1b[%d;%dH", term.cursor_y, term.cursor_x);
			write(STDOUT_FILENO, cursor_buffer, strlen(cursor_buffer));
			break;
		}
		case UPDATE_STATUS_BAR:
			Render_updateStatusBar();
			break;
		case UPDATE_TEXT:
			break;
		default: 
			break;
	}
} 

void Render_updateText() {
	
}

void Render_updateStatusBar() {
	// moves cursor to the bottom of the screen
	char cursor_buffer[16];
	snprintf(cursor_buffer, sizeof(cursor_buffer), "\x1b[%d;1H", term.screen_rows + 1);
	write(STDOUT_FILENO, cursor_buffer, strlen(cursor_buffer));

	// clears line
	write(STDOUT_FILENO, "\x1b[K", 3);
	
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
/*
void Render_screenBufferAppend(const char* string, int length) {
	char* new_buffer = realloc(screen_buffer.buffer, screen_buffer.length + length);

	if (new_buffer == NULL) return;
	memcpy(&new_buffer[screen_buffer.length], string, length); // copies in data to the very end of the buffer
	
	// updating variables
	screen_buffer.buffer = new_buffer;
	screen_buffer.length += length;
}
*/
void Render_destructor() {
	free(screen_buffer.buffer); 
	free(status_bar.buffer);
}


// rendering the interface
void Render_calendarGrid(int days, int start) {

}

void Render_schedule(char* info, int length) {

}
