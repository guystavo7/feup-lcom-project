#ifndef BASIC_TOWER
#define BASIC_TOWER

#include "genericTower.h"

GenericTower* basicTowerNew(Vec2d position);
void basicTowerUpdate(GenericTower* tower, Terminal* terminal, Arena* arena, float deltaTime);

#endif
