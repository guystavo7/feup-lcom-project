#include "../include/game.h"
#include "../include/inputHandler.h"
#include "../resources/1.h"
#include "../include/spriteXMP.h"

#include <stdio.h>

static SpriteXPM* sprite; 
static int x = 50;
static int y = 50;

void startGame() {
	sprite = newSpriteXPM((xpm_map_t)abc, XPM_8_8_8);
}

void exitGame() {
	freeSpriteXPM(sprite);
}

void gameLoop(Terminal* terminal) {
	if (getKeyPressed(getInstance(), KEYCODE_W)) y--;
	if (getKeyPressed(getInstance(), KEYCODE_S)) y++;
	if (getKeyPressed(getInstance(), KEYCODE_A)) x--;
	if (getKeyPressed(getInstance(), KEYCODE_D)) x++;

	if (x < 0) x = 0;
	if (x > 800) x = 800;
	if (y < 0) y = 0;
	if (y > 600) y = 600;

	drawSpriteXPM(sprite, terminal, x, y);
}

