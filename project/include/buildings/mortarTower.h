#ifndef MORTAR_TOWER 
#define MORTAR_TOWER 

#include "genericTower.h"

GenericTower* mortarTowerNew(Vec2d position);
void mortarTowerUpdate(GenericTower* tower, Terminal* terminal, Arena* arena, float deltaTime);

#endif
