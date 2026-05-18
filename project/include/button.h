#ifndef BUTTON
#define BUTTON

#include "../../lab5/terminal.h"

#include <stdbool.h>

void buttonInit(void);
void buttonFree(void);
void buttonDraw(Terminal* terminal);
void buttonOnMouseClick(int mx, int my);
bool buttonIsPopupOpen(void);

#endif
