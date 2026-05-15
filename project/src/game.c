#include "../include/game.h"
#include "../../lab5/spriteXMP.h"
#include "../include/animatedSprite.h"
#include "../include/arena.h"
#include "../include/inputHandler.h"
#include "../include/rigidBody.h"
#include "../include/cursorHandler.h"

#include "../resources/1.h"
#include "../resources/coinEdge.h"
#include "../resources/coinMid.h"
#include "../resources/coinMidDark.h"

static Arena* arena;
static SpriteXPM* mouse;
static RigidBody* rb;

static animatedSprite* playerSprite;

void startGame() {
    mouse = newSpriteXPM((xpm_map_t)abc, XPM_8_8_8);
    rb = rigidBodyNew(100, 100, 16, 16, 0);
    arena = arenaNew();

    SpriteXPM** frames = malloc(3 * sizeof(SpriteXPM*));
    frames[0] = newSpriteXPM((xpm_map_t)coin_f3_xpm, XPM_8_8_8);
    frames[1] = newSpriteXPM((xpm_map_t)coin_f2_xpm, XPM_8_8_8);
    frames[2] = newSpriteXPM((xpm_map_t)coin_f4_xpm, XPM_8_8_8);

    playerSprite = animatedSpriteNew(1.0f, frames, 3);
}

void exitGame() {
    animatedSpriteFree(playerSprite);
    freeSpriteXPM(mouse);
    rigidBodyFree(rb);
    arenaFree(arena);
}

void gameLoop(Terminal* terminal, float deltaTime) {
    int dirX = 0, dirY = 0;

    if (getKeyPressed(getInputHandler(), KEYCODE_W)) dirY = -1;
    if (getKeyPressed(getInputHandler(), KEYCODE_S)) dirY = 1;
    if (getKeyPressed(getInputHandler(), KEYCODE_A)) dirX = -1;
    if (getKeyPressed(getInputHandler(), KEYCODE_D)) dirX = 1;

    rigidBodySetVelocity(rb, 5 * dirX, 5 * dirY);
    rigidBodyUpdate(rb, arena, 1);
    animatedSpriteUpdate(rigidBodyGetX(rb), rigidBodyGetY(rb), playerSprite, terminal, deltaTime);

    int px, py;
    cursorSpaceToPixel(&px, &py);
    drawSpriteXPM(mouse, terminal, px, py);
}
