#include "../include/buildings/basicTower.h"
#include "../resources/building/buildings.h"

GenericTower* basicTowerNew(Vec2d position) {
	char** XPM = Tower;	
	Vec2d size = {43, 28};	
	Vec2d offset = {9, 60};
	
	return towerNew(XPM, position, size, offset, basicTowerUpdate);
}

void basicTowerUpdate(GenericTower* tower, Terminal* terminal, Arena* arena, float deltaTime) {
}
