#include "../include/game.h"
#include "../include/inputHandler.h"

#include <stdio.h>

void gameLoop() {
	if (getKeyDown(getInstance(), KEYCODE_4)) printf("Pressed 4\n");
}

