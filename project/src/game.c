#include "../include/game.h"
#include "../../lab5/spriteXMP.h"
#include "../include/animatedSprite.h"
#include "../include/arena.h"
#include "../include/inputHandler.h"
#include "../include/rigidBody.h"
#include "../include/cursorHandler.h"
#include "../include/arena.h"

#include "../resources/1.h"
#include "../resources/coinEdge.h"
#include "../resources/coinMid.h"
#include "../resources/coinMidDark.h"

static Arena* arena;
static SpriteXPM* mouse;

void startGame() {
    mouse = newSpriteXPM((xpm_map_t)abc, XPM_8_8_8, 0, 0);

    arena = arenaNew();

    SpriteXPM** frames = malloc(3 * sizeof(SpriteXPM*));
	int offsetX = 0;
	int offsetY = 0;

    frames[0] = newSpriteXPM((xpm_map_t)coin_f3_xpm, XPM_8_8_8, offsetX, offsetY);
    frames[1] = newSpriteXPM((xpm_map_t)coin_f2_xpm, XPM_8_8_8, offsetX, offsetY);
    frames[2] = newSpriteXPM((xpm_map_t)coin_f4_xpm, XPM_8_8_8, offsetX, offsetY);
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
