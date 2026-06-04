#ifndef BASIC_LONG_TOWER 
#define BASIC_LONG_TOWER 

#include "genericTower.h"

GenericTower* basicLongTowerNew(Vec2d position);
void basicLongTowerUpdate(GenericTower* tower, Terminal* terminal, Arena* arena, float deltaTime);

#endif
