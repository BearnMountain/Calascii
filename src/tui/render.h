#ifndef RENDER_H
#define RENDER_H

// ascii escapes
#define CLEAR_LINE "\x1b[K"
#define CLEAR_SCREEN "\x1b[2J"

// enums for updating screen
typedef enum {
	UPDATED_CURSOR,
	UPDATED_STATUS_BAR
} UpdateType;

struct ScreenBuffer { // one instance holds all the screen tui data
	char* buffer;
	int length;
};

struct ScreenBuffer_t {
	char** buffer;
	int max_length;
	int max_rows; // easier to refresh a row then the whole buffer
};

struct StatusBar {
	char* buffer;
	int buffer_length;
	int string_length;
};

extern struct ScreenBuffer screen_buffer;
extern struct StatusBar status_bar;

// setup
void Render_init();

// getters
int Render_getWindowSize(int* rows, int* cols);

// rendering to terminal
void Render_updateScreen(UpdateType u);
void Render_refreshScreen();
void Render_drawRows();

void Render_updateStatusBar();

// setting up screen buffer
void Render_screenBufferAppend(const char* string, int length);
void Render_destructor();

// creating the TUI
void Render_calendarGrid(int days, int start); // going to be 7x5 grid, just fill in days
void Render_schedule(char* info, int length); // custom info object 

#endif
