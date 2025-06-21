#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>

#include "keybind/keybinds.h"
#include "tui/render.h"
#include "tui/terminal.h"

/*

void enter_alternate_screen() {
    write(STDOUT_FILENO, "\033[?1049h", 8); // switch to alternate screen
}

void exit_alternate_screen() {
    write(STDOUT_FILENO, "\033[?1049l", 8); // switch back to main screen
}
*/

int main(void) {

	Terminal_enableRawMode();
	Terminal_init();
	Render_init();

	while (1) {
		Keybind_processKeypress();
	}

	// cleanup
	Render_destructor();
	Terminal_disableRawMode();

	return 0;
}
