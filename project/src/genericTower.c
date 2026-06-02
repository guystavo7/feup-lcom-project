#include "../include/buildings/genericTower.h"
#include "../include/components/boxCollider.h"
#include "../include/components/particle.h"
#include "../include/data.h"

#include "../resources/particles/dust/dust.h"
#include <stdlib.h>

struct GenericTower {
    SpriteXPM* upperHalf;
    SpriteXPM* lowerHalf;
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
    res->p = particleNew(DUST_FRAME_SPEED, sprites, DUST_FRAMES);
    particleStart(res->p);

    res->hitBox = boxColliderNew(position, hitBoxOffset, size, 0);
    res->position = position;

    res->towerUpdate = towerUpdate;
    return res;
}

void towerUpdate(GenericTower* tower, Terminal* terminal, Arena* arena, float deltaTime) {
	drawSpriteXPM(tower->lowerHalf, terminal, tower->position.x, tower->position.y, false);
    tower->towerUpdate(tower, terminal, arena, deltaTime);
    particleUpdate(tower->position.x, tower->position.y, tower->p, terminal, deltaTime, false);
}

void towerDrawUpperHalf(GenericTower *tower, Terminal *terminal) {
	drawSpriteXPM(tower->upperHalf, terminal, tower->position.x, tower->position.y, false);
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
