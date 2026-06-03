#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "../utils/vec2d.h"
#include "../../../lab5/spriteXMP.h"
#include "../../../lab5/terminal.h"
#include "boxCollider.h"

#include <stdbool.h>

typedef enum {
    PROJ_NORMAL,
    PROJ_PIERCING,
    PROJ_HOMING
} ProjectileType;

typedef struct projectile Projectile;

Projectile* projectileCreate(ProjectileType type, float x, float y, float vel_x, float vel_y, SpriteXPM* sprite);
void projectileFree(Projectile* p);

void projectileUpdate(Projectile* p, double delta_t, Terminal* terminal);

void projectileUpdateHomingTarget(Projectile* p, float target_x, float target_y);

float projectileGetX(Projectile* p);
float projectileGetY(Projectile* p);
float projectileGetVelX(Projectile* p);
float projectileGetVelY(Projectile* p);
ProjectileType projectileGetType(Projectile* p);

bool projectileIsActive(Projectile* p);
void projectileSetActive(Projectile* p, bool active);

BoxCollider* projectileGetCollider(Projectile* p);

int projectileGetPierces(Projectile* p);
void projectileAddPierce(Projectile* p);

void projectileSetMaxPierce(Projectile* p, int max);

void projectileUpdadeSpeed(Projectile* p, float incX, float incY);

#endif
