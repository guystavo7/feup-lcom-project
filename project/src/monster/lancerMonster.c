#include "../../include/monster/lancerMonster.h"

#include "../../resources/red/lancer/lancer.h"

#include <stdlib.h>

#define LANCER_RUN_FRAMES 6
#define LANCER_FRAME_SPEED 10

static AnimatedSprite* lancerAnimation = NULL;

static AnimatedSprite* getSprites_(char** XPMs[], int n, float frameSpeed, Vec2d offset) {
    SpriteXPM** res = malloc((unsigned int)n * sizeof(SpriteXPM*));
    for (int i = 0; i < n; i++) res[i] = newSpriteXPM((xpm_map_t)XPMs[i], XPM_8_8_8, offset.x, offset.y);
    return animatedSpriteNew(frameSpeed, res, (unsigned int)n, LOOP);
}

static AnimatedSprite* lancerAnimationBuilder(void) {
    lancerAnimation = getSprites_(lancerXPM, LANCER_RUN_FRAMES, LANCER_FRAME_SPEED, (Vec2d){0, 0});
    return lancerAnimation;
}

Monster* lancerMonsterNew(float x, float y, Path* path) {
    return monsterNew(x, y, 36.0f, 74.0f, LANCER_SPEED, LANCER_HEALTH, lancerAnimationBuilder, path);
}
