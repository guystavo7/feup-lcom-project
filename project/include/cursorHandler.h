#ifndef CURSOR_HANDLER
#define CURSOR_HANDLER

#include <stdint.h>
#include "../../lab5/spriteXMP.h"

void cursorInit(int w, int h);
void cursorReset();
void cursorUpdateCoordinates(int16_t dx, int16_t dy);
void cursorDraw(Terminal* terminal, SpriteXPM* sprite);

int cursorGetX();
int cursorGetY();

#endif

