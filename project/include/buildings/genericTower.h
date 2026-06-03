#ifndef GENERIC_TOWER
#define GENERIC_TOWER 

#include "../../../lab5/terminal.h"
#include "../../../lab5/spriteXMP.h"
#include "../components/animatedSprite.h"

#include "../arena.h"

typedef struct GenericTower GenericTower;

GenericTower* towerNew(SpriteXPM* upperHalf, SpriteXPM* lowerHalf, AnimatedSprite* archerIdle, AnimatedSprite* archerShoot, Vec2d position, Vec2d size, Vec2d hitBoxOffset, Vec2d rangeSize, Vec2d rangeOffset, void (*towerUpdate)(GenericTower* tower, Terminal* terminal, Arena* arena, float deltaTime), float cooldown, ProjectileType projectileType, SpriteXPM* arrowSprite);
void towerFree(GenericTower* tower);
BoxCollider* towerGetHitBox(GenericTower* tower);

void towerUpdate(GenericTower* tower, Terminal* terminal, Arena* arena, float deltaTime);
void towerDrawUpperHalf(GenericTower* tower, Terminal* terminal, float deltaTime);
bool towerAddProjectile(GenericTower* tower, Projectile* projectile);

#endif
