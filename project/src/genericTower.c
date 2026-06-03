#include "../include/buildings/genericTower.h"
#include "../include/components/animatedSprite.h"
#include "../include/components/boxCollider.h"
#include "../include/data.h"
#include "../include/utils/timer.h"

#include "../resources/particles/dust/dust.h"
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

    SpriteXPM* arrowSprite;

    void (*towerUpdate)(GenericTower* tower, Terminal* terminal, Arena* arena, float deltaTime);
};

GenericTower* towerNew(SpriteXPM* upperHalf, SpriteXPM* lowerHalf, AnimatedSprite* archerIdle, AnimatedSprite* archerShoot, Vec2d position, Vec2d size, Vec2d hitBoxOffset, Vec2d rangeSize, Vec2d rangeOffset, void (*towerUpdate)(GenericTower* tower, Terminal* terminal, Arena* arena, float deltaTime), float cooldown, ProjectileType projectileType, SpriteXPM* arrowSprite) {
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
    res->arrowSprite = arrowSprite;
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

        BoxCollider* colliders[10];
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
        if(dir.x > 0 && dir.y > 0) p = projectileCreate(tower->projectileType, tower->position.x, tower->position.y + ARCHER_OFFSET.y, dir.x * 500, dir.y * 500, tower->arrowSprite);
        else if (dir.x < 0 && dir.y > 0) p = projectileCreate(tower->projectileType, tower->position.x, tower->position.y + ARCHER_OFFSET.y, dir.x * 500, dir.y * 500, tower->arrowSprite);
        else if (dir.x < 0 && dir.y < 0) p = projectileCreate(tower->projectileType, tower->position.x, tower->position.y + ARCHER_OFFSET.y, dir.x * 500, dir.y * 500, tower->arrowSprite);
        else p = projectileCreate(tower->projectileType, tower->position.x, tower->position.y + ARCHER_OFFSET.y, dir.x * 500, dir.y * 500, tower->arrowSprite);

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
    free(tower->projectiles);

    free(tower);
}

BoxCollider* towerGetHitBox(GenericTower* tower) {
    return tower->hitBox;
}

bool towerAddProjectile(GenericTower* tower, Projectile* projectile) {
    if (tower->projectilesCount >= tower->projectilesCapacity) {
        return false;
    }
    tower->projectiles[tower->projectilesCount] = projectile;
    tower->projectilesCount++;
    return true;
}
