#ifndef PIERCING_TOWER 
#define PIERCING_TOWER 

#include "genericTower.h"

GenericTower* piercingTowerNew(Vec2d position);
void piercingTowerUpdate(GenericTower* tower, Terminal* terminal, Arena* arena, float deltaTime);

#endif
