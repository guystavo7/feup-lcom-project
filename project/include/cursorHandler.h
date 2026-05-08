#ifndef   CURSOR_HANDLER
#define CURSOR_HANDLER

#include <stdint.h>
void cursorInit(int w, int h);
void cursorReset();
void cursorUpdateCoordinates(int16_t du, int16_t dv);
void cursorSpaceToPixel(int* pu, int*pv);

int cursorGetU();
int cursorGetV();

#endif
