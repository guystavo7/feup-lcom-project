#include "../include/components/projectile.h"
#include <stdlib.h>
#include <math.h>

struct projectile {
    ProjectileType type;
    Vec2d* pos;
    Vec2d* vel;
    SpriteXPM* sprite;
    BoxCollider* collider;
    
    union {
        struct {
            int max_pierces;
            int current_pierces;
        } piercing;
        
        struct {
            float target_x;
            float target_y;
            float turn_speed;
        } homing;
    } extra_data;
    
    bool active;
};

Projectile* projectileCreate(ProjectileType type, float x, float y, float vel_x, float vel_y, SpriteXPM* sprite) {
    Projectile* p = malloc(sizeof(Projectile));
    if (!p) return NULL;
    
    p->type = type;
    p->pos = vec2dCreate(x, y);
    p->vel = vec2dCreate(vel_x, vel_y);
    p->sprite = sprite;
    p->active = true;
    
    int w = 16;
    int h = 16;
    if (sprite != NULL) {
        w = getSpriteWidth(sprite);
        h = getSpriteHeight(sprite);
    }
    
    Vec2d pos = {x, y};
    Vec2d offset = {0, 0};
    Vec2d size = {w, h};
    p->collider = boxColliderNew(pos, offset, size, 0);
    
    if (type == PROJ_PIERCING) {
        p->extra_data.piercing.max_pierces = 2;
        p->extra_data.piercing.current_pierces = 0;
    } else if (type == PROJ_HOMING) {
        p->extra_data.homing.target_x = x;
        p->extra_data.homing.target_y = y;
        p->extra_data.homing.turn_speed = 50.0f; 
    }
    
    return p;
}

void projectileFree(Projectile* p) {
    if (!p) return;
    vec2dFree(p->pos);
    vec2dFree(p->vel);
    boxColliderFree(p->collider);
    free(p);
}

void projectileUpdate(Projectile* p, double delta_t, Terminal* terminal) {
    if (!p->active) return;
    
    if (p->type == PROJ_HOMING) {
        //logica simples de virar para o target
        float dx = p->extra_data.homing.target_x - vec2dGetX(p->pos);
        float dy = p->extra_data.homing.target_y - vec2dGetY(p->pos);
        float dist = sqrt(dx*dx + dy*dy);
        
        if (dist > EPSILON) {
            float ndx = dx / dist;
            float ndy = dy / dist;
            
            float current_vel_x = vec2dGetX(p->vel);
            float current_vel_y = vec2dGetY(p->vel);
            float current_speed = sqrt(current_vel_x*current_vel_x + current_vel_y*current_vel_y);
            
            // Aproximar a velocidade da direção do target (interpolação simples)
            float turn = p->extra_data.homing.turn_speed * delta_t;
            float new_vel_x = ndx * turn;
            float new_vel_y = ndy * turn;
            
            // Normalizar a nova velocidade para manter o speed original
            float new_speed = sqrt(new_vel_x*new_vel_x + new_vel_y*new_vel_y);
            if (new_speed > EPSILON) {
                vec2dSetX(p->vel, (new_vel_x / new_speed) * current_speed);
                vec2dSetY(p->vel, (new_vel_y / new_speed) * current_speed);
            }
        }
    }
    
    Vec2d* displacement = vec2dCreate(vec2dGetX(p->vel), vec2dGetY(p->vel));
    vec2dMultiply(displacement, delta_t);
    vec2dAddVec(p->pos, displacement);
    vec2dFree(displacement);
    
    boxColliderSetX(p->collider, vec2dGetX(p->pos));
    boxColliderSetY(p->collider, vec2dGetY(p->pos));
    
    projectileUpdadeSpeed(p, 0.7, 0.7);

    if (p->sprite != NULL) {
        drawSpriteXPM(p->sprite, terminal, (int)vec2dGetX(p->pos), (int)vec2dGetY(p->pos), false);
    }
}

void projectileUpdateHomingTarget(Projectile* p, float target_x, float target_y) {
    if (p->type == PROJ_HOMING) {
        p->extra_data.homing.target_x = target_x;
        p->extra_data.homing.target_y = target_y;
    }
}

float projectileGetX(Projectile* p) {
    return vec2dGetX(p->pos);
}

float projectileGetY(Projectile* p) {
    return vec2dGetY(p->pos);
}

float projectileGetVelX(Projectile* p) {
    return vec2dGetX(p->vel);
}

float projectileGetVelY(Projectile* p) {
    return vec2dGetY(p->vel);
}

ProjectileType projectileGetType(Projectile* p) {
    return p->type;
}

bool projectileIsActive(Projectile* p) {
    return p->active;
}

void projectileSetActive(Projectile* p, bool active) {
    p->active = active;
}

BoxCollider* projectileGetCollider(Projectile* p) {
    return p->collider;
}

int projectileGetPierces(Projectile* p) {
    if (p->type == PROJ_PIERCING) {
        return p->extra_data.piercing.current_pierces;
    }
    return 0;
}

void projectileAddPierce(Projectile* p) {
    if (p->type == PROJ_PIERCING) {
        p->extra_data.piercing.current_pierces++;
        if (p->extra_data.piercing.current_pierces >= p->extra_data.piercing.max_pierces) {
            p->active = false;
        }
    } else {
        p->active = false;
    }
}

void projectileSetMaxPierce(Projectile* p, int max) {
    if (p->type == PROJ_PIERCING){
        p->extra_data.piercing.max_pierces = max;
    }
}

void projectileUpdadeSpeed(Projectile* p, float incX, float incY){
    vec2dSetX(p->vel, vec2dGetX(p->vel) + incX);
    vec2dSetY(p->vel, vec2dGetY(p->vel) + incY);
}
