#include "../include/buildings/genericTower.h"
#include "../include/components/animatedSprite.h"
#include "../include/components/boxCollider.h"
#include "../include/utils/timer.h"
#include "../include/data.h"

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

    Vec2d position;
    AnimatedSprite* p;

    enum TowerState state;
	Timer* cooldownTimer;

    void (*towerUpdate)(GenericTower* tower, Terminal* terminal, Arena* arena, float deltaTime);
};

GenericTower* towerNew(SpriteXPM* upperHalf, SpriteXPM* lowerHalf, AnimatedSprite* archerIdle, AnimatedSprite* archerShoot, Vec2d position, Vec2d size, Vec2d hitBoxOffset, Vec2d rangeSize, Vec2d rangeOffset, void (*towerUpdate)(GenericTower* tower, Terminal* terminal, Arena* arena, float deltaTime), float cooldown) {
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
    return res;
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
		return TOWER_SHOOT;
    }

    return tower->state;
}

static enum TowerState shootTransition(GenericTower* tower, Arena* arena, float deltaTime) {
	if (animatedSpriteIsFinished(tower->archerShoot)) {
		idleOnEnter(tower);
		return TOWER_IDLE;
	}

	return tower->state;
}

void towerUpdate(GenericTower* tower, Terminal* terminal, Arena* arena, float deltaTime) {
    enum TowerState newState = tower->state;
    drawRectangle(terminal, boxColliderGetX(tower->range) + boxColliderGetOffsetX(tower->range), boxColliderGetY(tower->range) + boxColliderGetOffsetY(tower->range), boxColliderGetWidth(tower->range), boxColliderGetHeight(tower->range), 0xFFFFFF);

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

void towerDrawUpperHalf(GenericTower* tower, Terminal* terminal, float deltaTime) {
    drawSpriteXPM(tower->upperHalf, terminal, tower->position.x, tower->position.y, false);
    animatedSpriteUpdate(tower->position.x, tower->position.y, tower->current, terminal, deltaTime, tower->isFlipped);
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
    free(tower);
}

BoxCollider* towerGetHitBox(GenericTower* tower) {
    return tower->hitBox;
}
