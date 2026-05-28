#ifndef CURSOR_HANDLER
#define CURSOR_HANDLER

#include <stdint.h>
#include <stdbool.h>
#include "../../../lab5/spriteXMP.h"

typedef struct Cursor Cursor;

static Cursor* cursor;
static bool isCursorCreated = false;

void cursorFree(Cursor* c);

Cursor* getCursor();
void cursorReset(Cursor* c);
void cursorUpdateData(Cursor* c, int dx, int dy, bool leftButton, bool rightButton, bool middleButton);
void cursorUpdate(Cursor* c, Terminal* terminal);

int cursorGetX(Cursor* c);
int cursorGetY(Cursor* c);

bool cursorIsLeftButtonPressed(Cursor* c);
bool cursorIsLeftButtonPressedDown(Cursor* c);

bool cursorIsMiddleButtonPressed(Cursor* c);
bool cursorIsMiddleButtonPressedDown(Cursor* c);

bool cursorIsRightButtonPressed(Cursor* c);
bool cursorIsRightButtonPressedDown(Cursor* c);

void cursorGetPixelPosition(Cursor* c, int* px, int* py);

#endif

