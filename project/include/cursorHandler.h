#ifndef CURSOR_HANDLER
#define CURSOR_HANDLER

#include <stdint.h>

void cursorInit(int w, int h);
void cursorReset();
void cursorUpdateCoordinates(int16_t dx, int16_t dy);
void cursorSpaceToPixel(int* px, int*py);

int cursorGetX();
int cursorGetY();

#endif
