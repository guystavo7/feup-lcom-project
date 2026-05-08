#ifndef ANIMATED_SPRITE
#define ANIMATED_SPRITE

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdbool.h>
#include "../../lab5/spriteXMP.h"
#include "../../lab5/terminal.h"

typedef struct {
    SpriteXPM** sprites;
    float frameSpeed;
    int n;
    int current;
    float elapsedTime;
} animatedSprite;

animatedSprite* newAnimatedSprite(float frameSpeed, SpriteXPM** xpm_array, int n);

void freeAnimatedSprite(animatedSprite* sprite);

void updateAnimatedSprite(int x, int y, animatedSprite* sprite, Terminal* t, float deltaTime);

#endif
