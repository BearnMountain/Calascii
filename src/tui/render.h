#ifndef RENDER_H
#define RENDER_H

// ascii escapes
#include <stdint.h>
#define CLEAR_LINE "\x1b[K"
#define CLEAR_SCREEN "\x1b[2J"
#define HIDE_CURSOR "\x1b[?25l"

// enums for updating screen
typedef enum {
	UPDATE_CURSOR,
	UPDATE_STATUS_BAR,
	UPDATE_TEXT,
} UpdateType;

struct ScreenBuffer {
	char** buffer;
	uint16_t max_x, max_y;
};

struct StatusBar {
	uint8_t active;
	char* buffer;
	uint16_t buffer_length; // entire line length
	uint16_t string_length; // length of the string contents inside of the buffer
};

extern struct ScreenBuffer screen_buffer;
extern struct StatusBar status_bar;

// setup
void Render_init();

// getters
int Render_getWindowSize(int* rows, int* cols);

// rendering to terminal
void Render_updateScreen(UpdateType u);
void Render_updateStatusBar();
void Render_updateScreenBuffer();
void Render_updateText();

// setting up screen buffer
// void Render_screenBufferAppend(const char* string, int length);
void Render_destructor();

// creating the TUI
void Render_calendarGrid(int days, int start); // going to be 7x5 grid, just fill in days
void Render_schedule(char* info, int length); // custom info object 

#endif
