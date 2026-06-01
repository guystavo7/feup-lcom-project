#include "../include/buildings/genericTower.h"
#include "../include/components/boxCollider.h"
#include "../include/components/particle.h"
#include "../include/components/animatedSprite.h"
#include "../include/data.h"

#include "../resources/particles/dust/dust.h"
#include "../resources/archer/idle/archerIdle.h"
#include <stdlib.h>

struct GenericTower {
    SpriteXPM* upperHalf;

    SpriteXPM* lowerHalf;
	AnimatedSprite* archer;

    BoxCollider* hitBox;
    Vec2d position;
    Particle* p;

    void (*towerUpdate)(GenericTower* tower, Terminal* terminal, Arena* arena, float deltaTime);
};

GenericTower* towerNew(SpriteXPM* upperHalf, SpriteXPM* lowerHalf, Vec2d position, Vec2d size, Vec2d hitBoxOffset, void (*towerUpdate)(GenericTower* tower, Terminal* terminal, Arena* arena, float deltaTime)) {
    GenericTower* res = malloc(sizeof(GenericTower));
	res->upperHalf = upperHalf;
	res->lowerHalf = lowerHalf;

    SpriteXPM** sprites = malloc(sizeof(SpriteXPM*) * DUST_FRAMES);
    for (int i = 0; i < DUST_FRAMES; i++) sprites[i] = newSpriteXPM((xpm_map_t)dustXPM[i], XPM_8_8_8, 0, hitBoxOffset.y);
    res->p = particleNew(DUST_FRAME_SPEED, sprites, DUST_FRAMES, true);
    particleStart(res->p);

    res->hitBox = boxColliderNew(position, hitBoxOffset, size, 0);
    res->position = position;

	SpriteXPM** archerSprites = malloc(sizeof(SpriteXPM*) * ARCHER_IDLE_FRAMES);
	for (int i = 0; i < ARCHER_IDLE_FRAMES; i++) archerSprites[i] = newSpriteXPM((xpm_map_t)archerIdleXPM[i], XPM_8_8_8, ARCHER_OFFSET.x, ARCHER_OFFSET.y);
	res->archer = animatedSpriteNew(ARCHER_FRAME_SPEED, archerSprites, ARCHER_IDLE_FRAMES);

    res->towerUpdate = towerUpdate;
    return res;
}

void towerUpdate(GenericTower* tower, Terminal* terminal, Arena* arena, float deltaTime) {
	drawSpriteXPM(tower->lowerHalf, terminal, tower->position.x, tower->position.y, false);
    tower->towerUpdate(tower, terminal, arena, deltaTime);
    particleUpdate(tower->position.x, tower->position.y, tower->p, terminal, deltaTime, false);
}

void towerDrawUpperHalf(GenericTower *tower, Terminal *terminal, float deltaTime) {
	drawSpriteXPM(tower->upperHalf, terminal, tower->position.x, tower->position.y, false);
	animatedSpriteUpdate(tower->position.x, tower->position.y, tower->archer, terminal, deltaTime, false);
}

void towerFree(GenericTower* tower) {
    particleFree(tower->p);
    freeSpriteXPM(tower->lowerHalf);
    freeSpriteXPM(tower->upperHalf);
    boxColliderFree(tower->hitBox);
    free(tower);
}

BoxCollider* towerGetHitBox(GenericTower* tower) {
    return tower->hitBox;
}
