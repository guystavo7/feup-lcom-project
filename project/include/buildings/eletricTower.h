#ifndef ELETRICT_TOWER 
#define ELETRICT_TOWER 

#include "genericTower.h"

GenericTower* eletrictTowerNew(Vec2d position);
void eletrictTowerUpdate(GenericTower* tower, Terminal* terminal, Arena* arena, float deltaTime);

#endif
