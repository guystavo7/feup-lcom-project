#include "../include/game.h"
#include "../include/rigidBody.h"
#include "../include/arena.h"

#include "../../lab5/spriteXMP.h"
#include "../include/inputHandler.h"
#include "../resources/1.h"

static Arena* arena;
static SpriteXPM* sprite;
static RigidBody* rb;

void startGame() {
    sprite = newSpriteXPM((xpm_map_t)abc, XPM_8_8_8);
	rb = rigidBodyNew(100, 100, 32, 32, 0);
	arena = arenaNew();
}

void exitGame() {
    freeSpriteXPM(sprite);
	rigidBodyFree(rb);
	arenaFree(arena);
}

void gameLoop(Terminal* terminal) {
	int dirX = 0, dirY = 0;

    if (getKeyPressed(getInputHandler(), KEYCODE_W)) dirY = -1;
    if (getKeyPressed(getInputHandler(), KEYCODE_S)) dirY = 1;
    if (getKeyPressed(getInputHandler(), KEYCODE_A)) dirX = -1;
    if (getKeyPressed(getInputHandler(), KEYCODE_D)) dirX = 1;

	rigidBodySetVelocity(rb, 5 * dirX, 5 * dirY);
	rigidBodyUpdate(rb, arena, 1);
    drawSpriteXPM(sprite, terminal, rigidBodyGetX(rb), rigidBodyGetY(rb));
}
