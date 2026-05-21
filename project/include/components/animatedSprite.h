#ifndef ANIMATED_SPRITE
#define ANIMATED_SPRITE

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdbool.h>

#include "../../../lab5/spriteXMP.h"
#include "../../../lab5/terminal.h"

typedef struct AnimatedSprite AnimatedSprite;

AnimatedSprite* animatedSpriteNew(float frameSpeed, SpriteXPM** xpm_array, int n);
void animatedSpriteFree(AnimatedSprite* sprite);
void animatedSpriteUpdate(int x, int y, AnimatedSprite* sprite, Terminal* t, float deltaTime, bool isFlipped);

#endif
