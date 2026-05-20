#include "../include/game.h"
#include "../../lab5/spriteXMP.h"
#include "../include/arena.h"
#include "../include/cursorHandler.h"

#include "../resources/1.h"

static Arena* arena;
static SpriteXPM* mouse;

void startGame() {
    mouse = newSpriteXPM((xpm_map_t)abc, XPM_8_8_8, 0, 0);
    arena = arenaNew();
}

void exitGame() {
    freeSpriteXPM(mouse);
    arenaFree(arena);
}

void gameLoop(Terminal* terminal, float deltaTime) {
	arenaUpdate(arena, terminal, deltaTime);

    int px, py;
    cursorSpaceToPixel(&px, &py);
    drawSpriteXPM(mouse, terminal, px, py);
}
