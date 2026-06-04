#include "../include/buildings/eletricTower.h"
#include "../include/data.h"
#include "../resources/yellow/tower/yellowTower.h"
#include "../resources/yellow/archer/archerIdle.h"
#include "../resources/yellow/archer/archerShoot.h"
#include "../resources/lightning/lightning.h"

GenericTower* eletrictTowerNew(Vec2d position) {
    SpriteXPM* lowerHalf = newSpriteXPM((xpm_map_t)towerXPM[0], XPM_8_8_8, 0, 0);
    SpriteXPM* upperHalf = newSpriteXPM((xpm_map_t)towerXPM[1], XPM_8_8_8, BASIC_UPPER_HALF_OFFSET.x, BASIC_UPPER_HALF_OFFSET.y);

    SpriteXPM** archerIdle = malloc(sizeof(SpriteXPM*) * ARCHER_IDLE_FRAMES);
    for (int i = 0; i < ARCHER_IDLE_FRAMES; i++) archerIdle[i] = newSpriteXPM((xpm_map_t)archerIdleXPM[i], XPM_8_8_8, ARCHER_OFFSET.x, ARCHER_OFFSET.y);
    AnimatedSprite* idle = animatedSpriteNew(ARCHER_FRAME_SPEED, archerIdle, ARCHER_IDLE_FRAMES, LOOP);

    SpriteXPM** archerShoot = malloc(sizeof(SpriteXPM*) * ARCHER_SHOOT_FRAMES);
    for (int i = 0; i < ARCHER_SHOOT_FRAMES; i++) archerShoot[i] = newSpriteXPM((xpm_map_t)archerShootXPM[i], XPM_8_8_8, ARCHER_OFFSET.x, ARCHER_OFFSET.y);
    AnimatedSprite* shoot = animatedSpriteNew(ELETRICT_TOWER_SHOOT_SPEED, archerShoot, ARCHER_SHOOT_FRAMES, NORMAL);

	SpriteXPM** projectileSprite = malloc(sizeof(SpriteXPM*) * LIGHTNING_FRAMES); 
	for (int i = 0; i < LIGHTNING_FRAMES; i++) projectileSprite[i] = newSpriteXPM((xpm_map_t)lightningXPM[i], XPM_8_8_8, 0, 0);

    return towerNew(upperHalf, lowerHalf, idle, shoot, position, BASIC_TOWER_SIZE, BASIC_TOWER_HIT_BOX_OFFSET, ELETRICT_TOWER_RANGE, ELETRICT_TOWER_RANGE_OFFSET, eletrictTowerUpdate, ELETRICT_COOLDOWN, PROJ_NEAR, projectileSprite, LIGHTNING_FRAMES, TOWER_KIND_ELETRICT);
}

void eletrictTowerUpdate(GenericTower* tower, Terminal* terminal, Arena* arena, float deltaTime) {}
