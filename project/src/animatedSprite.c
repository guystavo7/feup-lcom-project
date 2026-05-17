#include "../include/animatedSprite.h"
#include <stdint.h>
#include <stdlib.h>

struct AnimatedSprite {
    SpriteXPM** sprites;
    int n;

    float frameSpeed;
    int current;
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

void animatedSpriteUpdate(int x, int y, AnimatedSprite* sprite, Terminal* t, float deltaTime) {
    sprite->elapsedTime += deltaTime * sprite->frameSpeed;
    float frameDuration = 1.0f / sprite->frameSpeed;

    if (sprite->elapsedTime >= frameDuration) {
        sprite->elapsedTime = 0.0f;
        sprite->current = (sprite->current + 1) % sprite->n;
    }

    drawSpriteXPM(sprite->sprites[sprite->current], t, x, y);
}
