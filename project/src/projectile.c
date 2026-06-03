#include "../include/components/projectile.h"
#include <math.h>
#include <stdlib.h>

static void drawCircle(Terminal* terminal, int xm, int ym, int r, uint32_t color) {
    int x = -r, y = 0, err = 2 - 2 * r;
    do {
        drawPixel(terminal, xm - x, ym + y, color);
        drawPixel(terminal, xm - y, ym - x, color);
        drawPixel(terminal, xm + x, ym - y, color);
        drawPixel(terminal, xm + y, ym + x, color);
        r = err;
        if (r <= y) err += ++y * 2 + 1;
        if (r > x || err > y) err += ++x * 2 + 1;
    } while (x < 0);
}

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

        struct {
            float target_x;
            float target_y;
            float gravity;
        } sky_fall;
    } extra_data;

    bool active;
};

Projectile* projectileCreate(ProjectileType type, float x, float y, float vel_x, float vel_y, SpriteXPM* sprite) {
    Projectile* p = malloc(sizeof(Projectile));
    if (!p) return NULL;

    p->type = type;
    if (type == PROJ_SKY_FALL) {
        p->pos = vec2dCreate(x, y - 400.0f);
        p->vel = vec2dCreate(0.0f, 0.0f);
    } else {
        p->pos = vec2dCreate(x, y);
        p->vel = vec2dCreate(vel_x, vel_y);
    }
    p->sprite = sprite;
    p->active = true;

    int w = 16;
    int h = 16;
    if (sprite != NULL) {
        w = getSpriteWidth(sprite);
        h = getSpriteHeight(sprite);
    }

    Vec2d pos = {p->pos->x, p->pos->y};
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
    } else if (type == PROJ_SKY_FALL) {
        p->extra_data.sky_fall.target_x = x;
        p->extra_data.sky_fall.target_y = y;
        p->extra_data.sky_fall.gravity = 380.0f;
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
        // logica simples de virar para o target
        float dx = p->extra_data.homing.target_x - vec2dGetX(p->pos);
        float dy = p->extra_data.homing.target_y - vec2dGetY(p->pos);
        float dist = sqrt(dx * dx + dy * dy);

        if (dist > EPSILON) {
            float ndx = dx / dist;
            float ndy = dy / dist;

            float current_vel_x = vec2dGetX(p->vel);
            float current_vel_y = vec2dGetY(p->vel);
            float current_speed = sqrt(current_vel_x * current_vel_x + current_vel_y * current_vel_y);

            // Aproximar a velocidade da direção do target (interpolação simples)
            float turn = p->extra_data.homing.turn_speed * delta_t;
            float new_vel_x = ndx * turn;
            float new_vel_y = ndy * turn;

            // Normalizar a nova velocidade para manter o speed original
            float new_speed = sqrt(new_vel_x * new_vel_x + new_vel_y * new_vel_y);
            if (new_speed > EPSILON) {
                vec2dSetX(p->vel, (new_vel_x / new_speed) * current_speed);
                vec2dSetY(p->vel, (new_vel_y / new_speed) * current_speed);
            }
        }
    } else if (p->type == PROJ_SKY_FALL) {
        if (vec2dGetY(p->pos) >= p->extra_data.sky_fall.target_y) {
            p->active = false;
            return;
        }

        float current_vel_y = vec2dGetY(p->vel);
        float new_vel_y = current_vel_y + p->extra_data.sky_fall.gravity * delta_t;
        vec2dSetY(p->vel, new_vel_y);
    }

    Vec2d* displacement = vec2dCreate(vec2dGetX(p->vel), vec2dGetY(p->vel));
    vec2dMultiply(displacement, delta_t);
    vec2dAddVec(p->pos, displacement);
    vec2dFree(displacement);

    if (p->type == PROJ_SKY_FALL) {
        if (vec2dGetY(p->pos) >= p->extra_data.sky_fall.target_y) {
            vec2dSetY(p->pos, p->extra_data.sky_fall.target_y);
            vec2dSetX(p->pos, p->extra_data.sky_fall.target_x);
        }
    }

    boxColliderSetX(p->collider, vec2dGetX(p->pos));
    boxColliderSetY(p->collider, vec2dGetY(p->pos));

    if (p->type == PROJ_SKY_FALL) {
        int w = 16;
        int h = 16;
        if (p->sprite != NULL) {
            w = getSpriteWidth(p->sprite);
            h = getSpriteHeight(p->sprite);
        }
        int cx = (int)p->extra_data.sky_fall.target_x + w / 2;
        int cy = (int)p->extra_data.sky_fall.target_y + h / 2;
        drawCircle(terminal, cx, cy, w / 2, 0xFF0000);
    }

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

void projectileUpdateSkyFallTarget(Projectile* p, float target_x, float target_y) {
    if (p->type == PROJ_SKY_FALL) {
        p->extra_data.sky_fall.target_x = target_x;
        p->extra_data.sky_fall.target_y = target_y;
        vec2dSetX(p->pos, target_x);
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
    if (p->type == PROJ_PIERCING) {
        p->extra_data.piercing.max_pierces = max;
    }
}

void projectileUpdadeSpeed(Projectile* p, float incX, float incY) {
    if (p->type == PROJ_SKY_FALL) {
        vec2dSetX(p->vel, vec2dGetX(p->vel) + incX);
        vec2dSetY(p->vel, vec2dGetY(p->vel) + incY);
    }
}
