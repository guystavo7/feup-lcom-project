#ifndef GENERIC_TOWER
#define GENERIC_TOWER 

#include "../../../lab5/terminal.h"
#include "../../../lab5/spriteXMP.h"
#include "../components/animatedSprite.h"

#include "../arena.h"

#include <stdbool.h>

typedef struct GenericTower GenericTower;

typedef enum {
    TOWER_KIND_BASIC,
    TOWER_KIND_FAST,
    TOWER_KIND_PIERCING,
    TOWER_KIND_MORTAR,
    TOWER_KIND_ELETRICT,
	TOWER_KIND_LONG,
} TowerKind;

GenericTower* towerNew(SpriteXPM* upperHalf, SpriteXPM* lowerHalf, AnimatedSprite* archerIdle, AnimatedSprite* archerShoot, Vec2d position, Vec2d size, Vec2d hitBoxOffset, Vec2d rangeSize, Vec2d rangeOffset, void (*towerUpdate)(GenericTower* tower, Terminal* terminal, Arena* arena, float deltaTime), float cooldown, ProjectileType projectileType, SpriteXPM** arrowSprites, int n, TowerKind kind);
void towerFree(GenericTower* tower);
BoxCollider* towerGetHitBox(GenericTower* tower);

Vec2d towerGetPosition(GenericTower* tower);
TowerKind towerGetKind(GenericTower* tower);
bool towerIsBasic(GenericTower* tower);

void towerUpdate(GenericTower* tower, Terminal* terminal, Arena* arena, float deltaTime);
void towerDrawUpperHalf(GenericTower* tower, Terminal* terminal, Arena* arena, float deltaTime);
bool towerAddProjectile(GenericTower* tower, Projectile* projectile);

#endif
