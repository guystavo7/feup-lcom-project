#include "../../include/monster/bossMonster.h"

#include "../../resources/black/warrior/warriorRun.h"

#include <stdlib.h>

#define BOSS_RUN_FRAMES 6
#define BOSS_FRAME_SPEED 8

static AnimatedSprite* bossAnimation = NULL;

static AnimatedSprite* getSprites_(char** XPMs[], int n, float frameSpeed, Vec2d offset) {
    SpriteXPM** res = malloc((unsigned int)n * sizeof(SpriteXPM*));
    for (int i = 0; i < n; i++) res[i] = newSpriteXPM((xpm_map_t)XPMs[i], XPM_8_8_8, offset.x, offset.y);
    return animatedSpriteNew(frameSpeed, res, (unsigned int)n, LOOP);
}

static AnimatedSprite* bossAnimationBuilder(void) {
    bossAnimation = getSprites_(warriorRunXPM, BOSS_RUN_FRAMES, BOSS_FRAME_SPEED, (Vec2d){0, 0});
    return bossAnimation;
}

Monster* bossMonsterNew(float x, float y, Path* path) {
    /* Sprite sheets are ~279x273; collider scaled down for tower/path gameplay */
    return monsterNew(x, y, 90.0f, 88.0f, BOSS_SPEED, BOSS_HEALTH, bossAnimationBuilder, path);
}
