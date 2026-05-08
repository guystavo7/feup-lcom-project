#include "../include/game.h"
#include "../include/animatedSprite.h"
#include "../../lab5/spriteXMP.h"
#include "../../lab2/timer.h"
#include "../include/inputHandler.h"
#include "../resources/coinEdge.h"
#include "../resources/coinMid.h"
#include "../resources/coinMidDark.h"

#include <stdio.h>
#include <stdlib.h>

static animatedSprite* playerSprite;
static int x = 50;
static int y = 50;
static int prevCounter = 0;

void startGame() {
    // Create animation frames from coin sprites
    SpriteXPM** frames = malloc(3 * sizeof(SpriteXPM*));
    frames[0] = newSpriteXPM((xpm_map_t)coin_f3_xpm, XPM_8_8_8);
    frames[1] = newSpriteXPM((xpm_map_t)coin_f2_xpm, XPM_8_8_8);
    frames[2] = newSpriteXPM((xpm_map_t)coin_f4_xpm, XPM_8_8_8);
    
    // Create animation at 6 FPS for faster flip
    playerSprite = newAnimatedSprite(6.0f, frames, 3);
    
    prevCounter = timer_get_counter();
}

void exitGame() {
    freeAnimatedSprite(playerSprite);
}

void gameLoop(Terminal* terminal) {
    // Calculate deltaTime in seconds
    int currentCounter = timer_get_counter();
    int tickDiff = currentCounter - prevCounter;
    prevCounter = currentCounter;
    
    // Assuming timer frequency of 60 Hz (1 tick = ~16.67ms)
    float deltaTime = tickDiff / 60.0f;
    
    if (getKeyPressed(getInputHandler(), KEYCODE_W)) y--;
    if (getKeyPressed(getInputHandler(), KEYCODE_S)) y++;
    if (getKeyPressed(getInputHandler(), KEYCODE_A)) x--;
    if (getKeyPressed(getInputHandler(), KEYCODE_D)) x++;

    if (x < 0) x = 0;
    if (x > 1024) x = 1024;
    if (y < 0) y = 0;
    if (y > 768) y = 768;

    updateAnimatedSprite(x, y, playerSprite, terminal, deltaTime);
}
