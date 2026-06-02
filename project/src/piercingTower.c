#include "../include/buildings/piercingTower.h"
#include "../include/data.h"
#include "../resources/purple/tower/purpleTower.h"
#include "../resources/purple/archer/archerIdle.h"

GenericTower* piercingTowerNew(Vec2d position) {
    SpriteXPM* lowerHalf = newSpriteXPM((xpm_map_t)towerXPM[0], XPM_8_8_8, 0, 0);
    SpriteXPM* upperHalf = newSpriteXPM((xpm_map_t)towerXPM[1], XPM_8_8_8, BASIC_UPPER_HALF_OFFSET.x, BASIC_UPPER_HALF_OFFSET.y);

	SpriteXPM** archerSprites = malloc(sizeof(SpriteXPM*) * ARCHER_IDLE_FRAMES);
	for (int i = 0; i < ARCHER_IDLE_FRAMES; i++) archerSprites[i] = newSpriteXPM((xpm_map_t)archerIdleXPM[i], XPM_8_8_8, ARCHER_OFFSET.x, ARCHER_OFFSET.y);
	AnimatedSprite* archer = animatedSpriteNew(ARCHER_FRAME_SPEED, archerSprites, ARCHER_IDLE_FRAMES, LOOP);

    return towerNew(upperHalf, lowerHalf, archer, position, BASIC_TOWER_SIZE, BASIC_TOWER_HIT_BOX_OFFSET, piercingTowerUpdate);
}

void piercingTowerUpdate(GenericTower* tower, Terminal* terminal, Arena* arena, float deltaTime) {}
