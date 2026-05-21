#include "../include/components/animatedSprite.h"
#include <stdint.h>
#include <stdlib.h>

struct AnimatedSprite {
    SpriteXPM** sprites;
    int n;

    float frameSpeed;
    float current;
    float elapsedTime;
};

AnimatedSprite* animatedSpriteNew(float frameSpeed, SpriteXPM** xpm, int n) {
    AnimatedSprite* s = malloc(sizeof(AnimatedSprite));
    s->frameSpeed = frameSpeed;
    s->sprites = xpm;

    s->n = n;
    s->current = 0;
    s->elapsedTime = 0.0f;
    return s;
}

void animatedSpriteFree(AnimatedSprite* sprite) {
    for (int i = 0; i < sprite->n; i++) {
        freeSpriteXPM(sprite->sprites[i]);
    }

    free(sprite->sprites);
    free(sprite);
}

void animatedSpriteUpdate(int x, int y, AnimatedSprite* sprite, Terminal* t, float deltaTime, bool isFlipped) {
    sprite->current += deltaTime * sprite->frameSpeed;
	if (sprite->current >= sprite->n) sprite->current = 0;

	SpriteXPM* currentFrame = sprite->sprites[(int)sprite->current];
    drawSpriteXPM(currentFrame, t, x, y, isFlipped);
}
