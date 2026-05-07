#include "../include/animatedSprite.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

struct animatedSprite {
  SpriteXPM** sprites;
  float frameSpeed;
  int n;
  int current;
  float elapsedTime;
};

animatedSprite* newAnimatedSprite(float frameSpeed, SpriteXPM** xpm, int n) {
  animatedSprite* s = malloc(sizeof(animatedSprite));
  s->frameSpeed = frameSpeed;
  s->sprites = xpm;
  s->n = n;
  s->current = 0;
  s->elapsedTime = 0.0f;
  return s;
}

void freeAnimatedSprite(animatedSprite* sprite) {
  for (size_t i = 0; i < sprite->n; i++){
    freeSpriteXPM(sprite->sprites[i]);
  }
  free(sprite->sprites);
  free(sprite);
}

void updateAnimatedSprite(int x, int y, animatedSprite* sprite, Terminal* t, float deltaTime) {
  sprite->elapsedTime += deltaTime;
  
  float frameDuration = 1.0f / sprite->frameSpeed;
  
  if (sprite->elapsedTime >= frameDuration) {
    sprite->elapsedTime = 0.0f;
    sprite->current = (sprite->current + 1) % sprite->n;
  }
  
  drawSpriteXPM(sprite->sprites[sprite->current], t, x, y);
}
