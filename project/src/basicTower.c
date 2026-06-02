#include "../include/buildings/basicTower.h"
#include "../include/data.h"
#include "../resources/building/tower/blueTower.h"

GenericTower* basicTowerNew(Vec2d position) {
    SpriteXPM* lowerHalf = newSpriteXPM((xpm_map_t)blueTowerXPM[0], XPM_8_8_8, 0, 0);
    SpriteXPM* upperHalf = newSpriteXPM((xpm_map_t)blueTowerXPM[1], XPM_8_8_8, BASIC_UPPER_HALF_OFFSET.x, BASIC_UPPER_HALF_OFFSET.y);

    return towerNew(upperHalf, lowerHalf, position, BASIC_TOWER_SIZE, BASIC_TOWER_HIT_BOX_OFFSET, basicTowerUpdate);
}

void basicTowerUpdate(GenericTower* tower, Terminal* terminal, Arena* arena, float deltaTime) {}
