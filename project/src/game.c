#include "../include/game.h"
#include "../include/animatedSprite.h"
#include "../../lab5/spriteXMP.h"
#include "../../lab2/timer.h"
#include "../include/inputHandler.h"
#include "../resources/1.h"
#include <lcom/pixmap.h>

#include <stdio.h>
#include <stdlib.h>

static animatedSprite* playerSprite;
static int x = 50;
static int y = 50;
static int prevCounter = 0;

void startGame() {
    // Create animation frames from LCF XPM sprites
    SpriteXPM** frames = malloc(3 * sizeof(SpriteXPM*));
    frames[0] = newSpriteXPM((xpm_map_t)pic1, XPM_INDEXED);
    frames[1] = newSpriteXPM((xpm_map_t)pic2, XPM_INDEXED);
    frames[2] = newSpriteXPM((xpm_map_t)pic3, XPM_INDEXED);
    
    // Create animation at 2 FPS
    playerSprite = newAnimatedSprite(2.0f, frames, 3);
    
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
    if (x > 800) x = 800;
    if (y < 0) y = 0;
    if (y > 600) y = 600;

    updateAnimatedSprite(x, y, playerSprite, terminal, deltaTime);
}
