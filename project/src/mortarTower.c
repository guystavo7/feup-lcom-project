#include "../include/buildings/mortarTower.h"
#include "../include/data.h"
#include "../resources/black/monk/monkIdle.h"
#include "../resources/black/tower/blackTower.h"

GenericTower* mortarTowerNew(Vec2d position) {
    SpriteXPM* lowerHalf = newSpriteXPM((xpm_map_t)towerXPM[0], XPM_8_8_8, 0, 0);
    SpriteXPM* upperHalf = newSpriteXPM((xpm_map_t)towerXPM[1], XPM_8_8_8, BASIC_UPPER_HALF_OFFSET.x, BASIC_UPPER_HALF_OFFSET.y);

    SpriteXPM** archerSprites = (SpriteXPM**)malloc(sizeof(SpriteXPM*) * MONK_IDLE_FRAMES);
    for (int i = 0; i < ARCHER_IDLE_FRAMES; i++) archerSprites[i] = newSpriteXPM((xpm_map_t)monkIdleXPM[i], XPM_8_8_8, MONK_OFFSET.x, MONK_OFFSET.y);
    AnimatedSprite* archer = animatedSpriteNew(MONK_FRAME_SPEED, archerSprites, MONK_IDLE_FRAMES, LOOP);

    return towerNew(upperHalf, lowerHalf, archer, position, BASIC_TOWER_SIZE, BASIC_TOWER_HIT_BOX_OFFSET, mortarTowerUpdate);
}

void mortarTowerUpdate(GenericTower* tower, Terminal* terminal, Arena* arena, float deltaTime) {}
