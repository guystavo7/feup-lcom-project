#ifndef ANIMATED_SPRITE
#define ANIMATED_SPRITE

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdbool.h>

#include "../../../lab5/spriteXMP.h"
#include "../../../lab5/terminal.h"

typedef struct AnimatedSprite AnimatedSprite;
enum AnimatedSpriteType {
	LOOP, // Always visible and it loops
	ONE_SHOT, // Only visible when it is running
	NORMAL // Always visible but only runs when called
};

AnimatedSprite* animatedSpriteNew(float frameSpeed, SpriteXPM** xpm_array, int n, enum AnimatedSpriteType type);
void animatedSpriteFree(AnimatedSprite* sprite);
void animatedSpriteUpdate(int x, int y, AnimatedSprite* sprite, Terminal* t, float deltaTime, bool isFlipped);

void animatedSpriteStart(AnimatedSprite* sprite);
bool animatedSpriteIsFinished(AnimatedSprite* sprite);
void animatedSpriteReset(AnimatedSprite* sprite);
int animatedSpriteGetWidth(AnimatedSprite* sprite);
int animatedSpriteGetHeight(AnimatedSprite* sprite);

#endif
