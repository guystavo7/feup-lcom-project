#ifndef ANIMATED_SPRITE
#define ANIMATED_SPRITE

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdbool.h>
#include "../../lab5/spriteXMP.h"
#include "../../lab5/terminal.h"

typedef struct animatedSprite animatedSprite;

animatedSprite* newAnimatedSprite(float frameSpeed, SpriteXPM* xpm);

void freeAnimatedSprite();

void updateAnimatedSprite(int x, int y, animatedSprite* sprite, Terminal* t, float deltaTime);

#endif
