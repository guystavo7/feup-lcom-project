#include "../include/utils/cursorHandler.h"
#include "../resources/cursor/cursor.h"
#include <stdlib.h>

enum keyState {
    UP,
    DOWN,
    PRESSED
};

enum MouseState {
	DEFAULT,
	CLICKED,
	DENIED,
};

struct Cursor {
    int x, y;
    int deltaX, deltaY;
    int width, height;
    int center[2];

    enum keyState leftButton;
    enum keyState rightButton;
    enum keyState middleButton;

	enum MouseState currentState;
    SpriteXPM** sprites;
};

static Cursor* cursorNew(int w, int h) {
    Cursor* res = malloc(sizeof(Cursor));

	res->currentState = DEFAULT;
    res->sprites = malloc(sizeof(SpriteXPM*) * 3);
	for (int i = 0; i < 3; i++) res->sprites[i] = newSpriteXPM((xpm_map_t)cursorsXPM[i], XPM_8_8_8, 0, 0);

    res->center[0] = w / 2;
    res->center[1] = h / 2;

    res->deltaX = 0;
    res->deltaY = 0;

    res->x = 0;
    res->y = 0;

    res->width = w;
    res->height = h;
    return res;
}

Cursor* getCursor() {
    if (!isCursorCreated) {
        cursor = cursorNew(1280, 1024);
        isCursorCreated = true;
    }

    return cursor;
}

void cursorFree(Cursor* c) {
	for (int i = 0; i < 3; i++) freeSpriteXPM(c->sprites[i]);
	free(c->sprites);
    free(c);
}

void cursorReset(Cursor* c) {
    c->x = 0;
    c->y = 0;
}

void cursorUpdateData(Cursor* c, int dx, int dy, bool leftButton, bool rightButton, bool middleButton) {
    c->deltaX += dx;
    c->deltaY += dy;

    if (leftButton && c->leftButton == UP) {
        c->leftButton = DOWN;
		c->currentState = CLICKED;
	} else if (leftButton) {
        c->leftButton = UP;
	}

    if (middleButton && c->middleButton == UP) {
		c->middleButton = DOWN;
	} else if (middleButton) {
        c->middleButton = UP;
	}

    if (rightButton && c->rightButton == UP) {
        c->rightButton = DOWN;
	} else if (rightButton) {
        c->rightButton = UP;
	}

	if (!leftButton) c->currentState = DEFAULT;
}

static void cursorUpdateCoordinates(Cursor* c) {
    int nextX = c->x + c->deltaX;
    int nextY = c->y + c->deltaY;

    c->deltaX = 0;
    c->deltaY = 0;

    int px = c->center[0] + nextX;
    if (px < 0)
        c->x = -c->center[0];
    else if (px > c->width - 1)
        c->x = (c->width - 1) - c->center[0];
    else
        c->x = nextX;

    int pv = c->center[1] + nextY;
    if (pv < 0)
        c->y = -c->center[1];
    else if (pv > c->height - 1)
        c->y = (c->height - 1) - c->center[1];
    else
        c->y = nextY;
}

static void cursorDraw(Cursor* c, Terminal* terminal) {
    int px = c->center[0] + c->x;
    int py = c->center[1] + c->y;
    drawSpriteXPM(c->sprites[c->currentState], terminal, px, py, false);
}

void cursorUpdate(Cursor* c, Terminal* terminal) {
    cursorUpdateCoordinates(c);
    cursorDraw(c, terminal);
}

void cursorGetPixelPosition(Cursor* c, int* px, int* py) {
    *px = c->center[0] + c->x;
    *py = c->center[1] + c->y;
}

int cursorGetX(Cursor* c) {
    return c->x;
}

int cursorGetY(Cursor* c) {
    return c->y;
}

bool cursorIsLeftButtonPressed(Cursor* c) {
    if (c->leftButton == DOWN) {
        c->leftButton = PRESSED;
    }

    return c->leftButton == PRESSED;
}

bool cursorIsLeftButtonPressedDown(Cursor* c) {
    if (c->leftButton == DOWN) {
        c->leftButton = PRESSED;
        return true;
    }

    return false;
}

bool cursorIsMiddleButtonPressed(Cursor* c) {
    if (c->middleButton == DOWN) {
        c->middleButton = PRESSED;
    }

    return c->middleButton == PRESSED;
}

bool cursorIsMiddleButtonPressedDown(Cursor* c) {
    if (c->middleButton == DOWN) {
        c->middleButton = PRESSED;
        return true;
    }

    return false;
}

bool cursorIsRightButtonPressed(Cursor* c) {
    if (c->rightButton == DOWN) {
        c->rightButton = PRESSED;
    }

    return c->rightButton == PRESSED;
}

bool cursorIsRightButtonPressedDown(Cursor* c) {
    if (c->rightButton == DOWN) {
        c->rightButton = PRESSED;
        return true;
    }

    return false;
}
