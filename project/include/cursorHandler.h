#ifndef CURSOR_HANDLER
#define CURSOR_HANDLER

#include <stdint.h>
#include "../../lab5/spriteXMP.h"

typedef struct Cursor Cursor;

static Cursor* cursor;
static bool isCursorCreated = false;

void cursorFree(Cursor* c);

Cursor* getCursor();
void cursorReset(Cursor* c);
void cursorUpdateCoordinates(Cursor* c, int dx, int dy);
void cursorDraw(Cursor* c, Terminal* terminal);

int cursorGetX(Cursor* c);
int cursorGetY(Cursor* c);
void cursorGetPixelPosition(Cursor* c, int* px, int* py);

#endif

