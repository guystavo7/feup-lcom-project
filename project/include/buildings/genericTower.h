#ifndef GENERIC_TOWER
#define GENERIC_TOWER 

#include "../../../lab5/terminal.h"
#include "../arena.h"

typedef struct GenericTower GenericTower;

GenericTower* towerNew(char* XPM[], Vec2d position, Vec2d size, Vec2d hitBoxOffset, void (*towerUpdate)(GenericTower* tower, Terminal* terminal, Arena* arena, float deltaTime));
void towerFree(GenericTower* tower);
BoxCollider* towerGetHitBox(GenericTower* tower);

void towerUpdate(GenericTower* tower, Terminal* terminal, Arena* arena, float deltaTime);

#endif
