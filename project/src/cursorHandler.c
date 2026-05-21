#include "../include/cursorHandler.h"
#include "../resources/cursor.h"
#include <stdlib.h>

struct Cursor {
    int x, y;
    int width, height;
    int center[2];

	SpriteXPM* sprite;
};

static Cursor* cursorNew(int w, int h) {
	Cursor* res = malloc(sizeof(Cursor));
	res->sprite = newSpriteXPM((xpm_map_t)cursor_xpm, XPM_8_8_8, 0, 0);
	res->center[0] = w / 2; 
	res->center[1] = h / 2; 
	
	res->x = 0;
	res->y = 0;

	res->width = w;
	res->height = h;
	return res;
}

Cursor* getCursor() {
	if (!isCursorCreated) {
		cursor = cursorNew(800, 600);
		isCursorCreated = true;
	}	

	return cursor;
}

void cursorFree(Cursor *c) {
	freeSpriteXPM(c->sprite);
	free(c);
}

void cursorReset(Cursor* c) {
    c->x = 0;
    c->y = 0;
}

void cursorUpdateCoordinates(Cursor* c, int dx, int dy) {
    int nextX = c->x + dx;
    int nextY = c->y + dy;

    int px = c->center[0] + nextX;
    if (px < 0) c->x = -c->center[0];
    else if (px > c->width - 1) c->x = (c->width - 1) - c->center[0];
    else c->x = nextX;

    int pv = c->center[1] + nextY;
    if (pv < 0) c->y = -c->center[1];
    else if (pv > c->height - 1) c->y = (c->height - 1) - c->center[1];
    else c->y = nextY;
}

void cursorDraw(Cursor* c, Terminal* terminal) {
    int px = c->center[0] + c->x;
    int py = c->center[1] + c->y;
    drawSpriteXPM(c->sprite, terminal, px, py, false);
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
