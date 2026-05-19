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
static RigidBody* rb;

static BoxCollider* hitBox;
static Vec2d playerPos = {100, 100};

static AnimatedSprite* playerSprite;

void startGame() {
    mouse = newSpriteXPM((xpm_map_t)abc, XPM_8_8_8);

	Vec2d playerSize = {16, 16};
	Vec2d offset = {0, 0};
	hitBox = boxColliderNew(playerPos, offset, playerSize, 0);

    rb = rigidBodyNew(playerPos, offset, playerSize, 0);
    arena = arenaNewFromFile("/home/lcom/labs/project/resources/arenaTest.txt");

    SpriteXPM** frames = malloc(3 * sizeof(SpriteXPM*));
	// int offsetX = 0;
	// int offsetY = 0;

    frames[0] = newSpriteXPM((xpm_map_t)coin_f3_xpm, XPM_8_8_8);
    frames[2] = newSpriteXPM((xpm_map_t)coin_f4_xpm, XPM_8_8_8);
    frames[1] = newSpriteXPM((xpm_map_t)coin_f2_xpm, XPM_8_8_8);

    playerSprite = animatedSpriteNew(1.0f, frames, 3);
}

void exitGame() {
    animatedSpriteFree(playerSprite);
    freeSpriteXPM(mouse);
    rigidBodyFree(rb);
	boxColliderFree(hitBox);
    arenaFree(arena);
}

void gameLoop(Terminal* terminal, float deltaTime) {
    int dirX = 0, dirY = 0;
	int playerSpeed = 400;

    if (getKeyPressed(getInputHandler(), KEYCODE_W)) dirY = -1;
    if (getKeyPressed(getInputHandler(), KEYCODE_S)) dirY = 1;
    if (getKeyPressed(getInputHandler(), KEYCODE_A)) dirX = -1;
    if (getKeyPressed(getInputHandler(), KEYCODE_D)) dirX = 1;

    rigidBodySetVelocity(rb, playerSpeed * dirX, playerSpeed * dirY);
    rigidBodyUpdate(rb, &playerPos, arena, hitBox, deltaTime);
    animatedSpriteUpdate(playerPos.x, playerPos.y, playerSprite, terminal, deltaTime);

    int px, py;
    cursorSpaceToPixel(&px, &py);
    drawSpriteXPM(mouse, terminal, px, py);
}
