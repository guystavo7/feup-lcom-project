#include "../include/buildings/basicTower.h"
#include "../resources/building/tower/blueTower.h"

GenericTower* basicTowerNew(Vec2d position) {
	Vec2d size = {49, 32};	
	Vec2d offset = {0, 0};
	SpriteXPM* lowerHalf = newSpriteXPM((xpm_map_t)blueTowerXPM[0], XPM_8_8_8, 0, 0);
	SpriteXPM* upperHalf = newSpriteXPM((xpm_map_t)blueTowerXPM[1], XPM_8_8_8, -6, -60);
	
	return towerNew(upperHalf, lowerHalf, position, size, offset, basicTowerUpdate);
}

void basicTowerUpdate(GenericTower* tower, Terminal* terminal, Arena* arena, float deltaTime) {
}
