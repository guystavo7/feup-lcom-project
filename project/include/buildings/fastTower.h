#ifndef FAST_TOWER 
#define FAST_TOWER 

#include "genericTower.h"

GenericTower* fastTowerNew(Vec2d position);
void fastTowerUpdate(GenericTower* tower, Terminal* terminal, Arena* arena, float deltaTime);

#endif
