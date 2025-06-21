#ifndef KEYBIND_H
#define KEYBIND_H

char Keybind_readKey();
void Keybind_processKeypress();

// key func
void Keybind_moveCursor(char key);
void Keybind_updateStatusBar(char key);

#endif
