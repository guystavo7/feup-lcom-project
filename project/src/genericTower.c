#include "../include/buildings/genericTower.h"
#include "../include/components/animatedSprite.h"
#include "../include/components/boxCollider.h"
#include "../include/data.h"
#include "../include/utils/timer.h"
#include "../include/utils/utils.h"

#include "../resources/particles/dust/dust.h"
#include <stdbool.h>
#include <stdlib.h>

enum TowerState {
    TOWER_IDLE,
    TOWER_SHOOT,
};

struct GenericTower {
    SpriteXPM* upperHalf;
    SpriteXPM* lowerHalf;

    AnimatedSprite* current;
    AnimatedSprite* archerIdle;
    AnimatedSprite* archerShoot;
    bool isFlipped;

    BoxCollider* hitBox;
    BoxCollider* range;
    BoxCollider* target;

    Vec2d position;
    AnimatedSprite* p;

    enum TowerState state;
    Timer* cooldownTimer;

    ProjectileType projectileType;
    int projectilesCapacity;
    int projectilesCount;
    Projectile** projectiles;

    SpriteXPM** arrowSprites;
    int arrowSpritesN;

    void (*towerUpdate)(GenericTower* tower, Terminal* terminal, Arena* arena, float deltaTime);

    TowerKind kind;
};

GenericTower* towerNew(SpriteXPM* upperHalf, SpriteXPM* lowerHalf, AnimatedSprite* archerIdle, AnimatedSprite* archerShoot, Vec2d position, Vec2d size, Vec2d hitBoxOffset, Vec2d rangeSize, Vec2d rangeOffset, void (*towerUpdate)(GenericTower* tower, Terminal* terminal, Arena* arena, float deltaTime), float cooldown, ProjectileType projectileType, SpriteXPM** arrowSprites, int n, TowerKind kind) {
    GenericTower* res = malloc(sizeof(GenericTower));
    res->upperHalf = upperHalf;
    res->lowerHalf = lowerHalf;

    SpriteXPM** sprites = malloc(sizeof(SpriteXPM*) * DUST_FRAMES);
    for (int i = 0; i < DUST_FRAMES; i++) sprites[i] = newSpriteXPM((xpm_map_t)dustXPM[i], XPM_8_8_8, 0, hitBoxOffset.y);
    res->p = animatedSpriteNew(DUST_FRAME_SPEED, sprites, DUST_FRAMES, ONE_SHOT);
    animatedSpriteStart(res->p);

    res->hitBox = boxColliderNew(position, hitBoxOffset, size, 0);
    res->range = boxColliderNew(position, rangeOffset, rangeSize, 0);
    res->position = position;

    res->archerIdle = archerIdle;
    res->archerShoot = archerShoot;
    res->isFlipped = false;

    res->towerUpdate = towerUpdate;
    res->state = TOWER_IDLE;
    res->current = archerIdle;

    res->cooldownTimer = timerNew(cooldown);
    res->projectileType = projectileType;
    res->arrowSprites = arrowSprites;
    res->arrowSpritesN = n;
    res->kind = kind;

    res->projectilesCapacity = 100;
    res->projectilesCount = 0;
    res->projectiles = malloc(sizeof(Projectile*) * res->projectilesCapacity);
    return res;
}

static void genericTowerProjectileHandler(GenericTower* tower, float deltaTime, Terminal* terminal, Arena* arena) {
    int activeCount = 0;
    for (int i = 0; i < tower->projectilesCount; i++) {
        if (projectileIsActive(tower->projectiles[i])) {
            tower->projectiles[activeCount++] = tower->projectiles[i];
        } else {
            projectileFree(tower->projectiles[i]);
        }
    }
    tower->projectilesCount = activeCount;

    for (int i = 0; i < tower->projectilesCount; i++) {
        Projectile* p = tower->projectiles[i];
        projectileUpdate(p, deltaTime, terminal);

        BoxCollider* colliders[1024];
        int n = arenaDetectCollisions(arena, projectileGetCollider(p), MONSTER_TAG, colliders);
        if (n > 0) {
            projectileSetActive(p, false);
        }
    }
}

// State Machine
// Idle
static void idleOnEnter(GenericTower* tower) {
    tower->current = tower->archerIdle;
    timerRestart(tower->cooldownTimer);
}

static void shootOnEnter(GenericTower* tower) {
    animatedSpriteStart(tower->archerShoot);
    tower->current = tower->archerShoot;
}

static void idleAct(GenericTower* tower, Terminal* terminal, Arena* arena, float deltaTime) {
    drawSpriteXPM(tower->lowerHalf, terminal, tower->position.x, tower->position.y, false);
    tower->towerUpdate(tower, terminal, arena, deltaTime);
    animatedSpriteUpdate(tower->position.x, tower->position.y, tower->p, terminal, deltaTime, false);

    timerUpdate(tower->cooldownTimer, deltaTime);
}

static void shootAct(GenericTower* tower, Terminal* terminal, Arena* arena, float deltaTime) {
    drawSpriteXPM(tower->lowerHalf, terminal, tower->position.x, tower->position.y, false);
    tower->towerUpdate(tower, terminal, arena, deltaTime);
    animatedSpriteUpdate(tower->position.x, tower->position.y, tower->p, terminal, deltaTime, false);
}

static enum TowerState idleTransition(GenericTower* tower, Arena* arena, float deltaTime) {
    BoxCollider* target = arenaFindFirstEnemy(arena, tower->range);
    if (!timerIsRunning(tower->cooldownTimer) && target) {
        tower->isFlipped = tower->position.x > boxColliderGetX(target);
        shootOnEnter(tower);
        tower->target = target;

        return TOWER_SHOOT;
    }

    return tower->state;
}

static enum TowerState shootTransition(GenericTower* tower, Arena* arena, float deltaTime) {
    if (animatedSpriteIsFinished(tower->archerShoot)) {
        idleOnEnter(tower);
        Vec2d dir = {boxColliderGetX(tower->target) - tower->position.x, boxColliderGetY(tower->target) - tower->position.y - ARCHER_OFFSET.y};
        vec2dNormalize(&dir);

        Projectile* p;
        Vec2d position = {tower->position.x, tower->position.y + ARCHER_OFFSET.y};
        int index = 0;

		// Hozizontal+
        if (floatModule(dir.y) < ARROW_EPSILON && dir.x > 0) {
            index = 0;
		// Hozizontal-
        } else if (floatModule(dir.y) < ARROW_EPSILON && dir.x < 0) {
            index = 4;
		// Vertical+
        } else if (floatModule(dir.x) < ARROW_EPSILON && dir.y > 0) {
            index = 6;
		// Vertical-
        } else if (floatModule(dir.x) < ARROW_EPSILON && dir.y < 0) {
            index = 2;
		// First quadrant
        } else if (dir.x > 0 && dir.y < 0) {
            index = 1;
		// Second quadrant
        } else if (dir.x < 0 && dir.y < 0) {
            index = 3;
		// Third quadrant
        } else if (dir.x > 0 && dir.y > 0) {
            index = 7;
        } else {
		// Forth quadrant
            index = 5;
        }

		float maxVelocity = ARROW_SPEED;
		if (tower->projectileType == PROJ_SKY_FALL) {
			position = (Vec2d){boxColliderGetX(tower->target), boxColliderGetY(tower->target)};
			maxVelocity = SKY_FALL_ARROW_SPEED;
		}

        Vec2d velocity = {dir.x * maxVelocity, dir.y * maxVelocity};
        p = projectileCreate(tower->projectileType, position.x, position.y, velocity.x, velocity.y, tower->arrowSprites[index]);
        if (!towerAddProjectile(tower, p)) projectileFree(p);
        return TOWER_IDLE;
    }

    return tower->state;
}

void towerUpdate(GenericTower* tower, Terminal* terminal, Arena* arena, float deltaTime) {
    enum TowerState newState = tower->state;
    // drawRectangle(terminal, boxColliderGetX(tower->range) + boxColliderGetOffsetX(tower->range), boxColliderGetY(tower->range) + boxColliderGetOffsetY(tower->range), boxColliderGetWidth(tower->range), boxColliderGetHeight(tower->range), 0xFFFFFF);
    switch (tower->state) {
        case TOWER_IDLE:
            idleAct(tower, terminal, arena, deltaTime);
            newState = idleTransition(tower, arena, deltaTime);
            break;
        case TOWER_SHOOT:
            shootAct(tower, terminal, arena, deltaTime);
            newState = shootTransition(tower, arena, deltaTime);
            break;
    }

    tower->state = newState;
}

void towerDrawUpperHalf(GenericTower* tower, Terminal* terminal, Arena* arena, float deltaTime) {
    drawSpriteXPM(tower->upperHalf, terminal, tower->position.x, tower->position.y, false);
    animatedSpriteUpdate(tower->position.x, tower->position.y, tower->current, terminal, deltaTime, tower->isFlipped);
    genericTowerProjectileHandler(tower, deltaTime, terminal, arena);
}

void towerFree(GenericTower* tower) {
    animatedSpriteFree(tower->p);
    animatedSpriteFree(tower->archerIdle);
    animatedSpriteFree(tower->archerShoot);

    freeSpriteXPM(tower->lowerHalf);
    freeSpriteXPM(tower->upperHalf);

    boxColliderFree(tower->hitBox);
    boxColliderFree(tower->range);

    timerFree(tower->cooldownTimer);

    for (int i = 0; i < tower->projectilesCount; i++) projectileFree(tower->projectiles[i]);
    for (int i = 0; i < tower->arrowSpritesN; i++) freeSpriteXPM(tower->arrowSprites[i]);
    free(tower->arrowSprites);
    free(tower->projectiles);
    free(tower);
}

BoxCollider* towerGetHitBox(GenericTower* tower) {
    return tower->hitBox;
}

Vec2d towerGetPosition(GenericTower* tower) {
    return tower->position;
}

TowerKind towerGetKind(GenericTower* tower) {
    return tower->kind;
}

bool towerIsBasic(GenericTower* tower) {
    return tower->kind == TOWER_KIND_BASIC;
}

bool towerAddProjectile(GenericTower* tower, Projectile* projectile) {
    if (tower->projectilesCount >= tower->projectilesCapacity) {
        return false;
    }
    tower->projectiles[tower->projectilesCount] = projectile;
    tower->projectilesCount++;
    return true;
}
