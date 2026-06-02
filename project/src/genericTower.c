#include "../include/buildings/genericTower.h"
#include "../include/components/boxCollider.h"
#include "../include/components/animatedSprite.h"
#include "../include/data.h"

#include "../resources/particles/dust/dust.h"
#include "../resources/blue/archer/archerIdle.h"
#include <stdlib.h>

struct GenericTower {
    SpriteXPM* upperHalf;

    SpriteXPM* lowerHalf;
	AnimatedSprite* archer;

    BoxCollider* hitBox;
    Vec2d position;
    AnimatedSprite* p;

    void (*towerUpdate)(GenericTower* tower, Terminal* terminal, Arena* arena, float deltaTime);
};

GenericTower* towerNew(SpriteXPM* upperHalf, SpriteXPM* lowerHalf, AnimatedSprite* archer, Vec2d position, Vec2d size, Vec2d hitBoxOffset, void (*towerUpdate)(GenericTower* tower, Terminal* terminal, Arena* arena, float deltaTime)) {
    GenericTower* res = malloc(sizeof(GenericTower));
	res->upperHalf = upperHalf;
	res->lowerHalf = lowerHalf;

    SpriteXPM** sprites = malloc(sizeof(SpriteXPM*) * DUST_FRAMES);
    for (int i = 0; i < DUST_FRAMES; i++) sprites[i] = newSpriteXPM((xpm_map_t)dustXPM[i], XPM_8_8_8, 0, hitBoxOffset.y);
    res->p = animatedSpriteNew(DUST_FRAME_SPEED, sprites, DUST_FRAMES, ONE_SHOT);
    animatedSpriteStart(res->p);

    res->hitBox = boxColliderNew(position, hitBoxOffset, size, 0);
    res->position = position;
	res->archer = archer;

    res->towerUpdate = towerUpdate;
    return res;
}

void towerUpdate(GenericTower* tower, Terminal* terminal, Arena* arena, float deltaTime) {
	drawSpriteXPM(tower->lowerHalf, terminal, tower->position.x, tower->position.y, false);
    tower->towerUpdate(tower, terminal, arena, deltaTime);
    animatedSpriteUpdate(tower->position.x, tower->position.y, tower->p, terminal, deltaTime, false);
}

void towerDrawUpperHalf(GenericTower *tower, Terminal *terminal, float deltaTime) {
	drawSpriteXPM(tower->upperHalf, terminal, tower->position.x, tower->position.y, false);
	animatedSpriteUpdate(tower->position.x, tower->position.y, tower->archer, terminal, deltaTime, false);
}

void towerFree(GenericTower* tower) {
    animatedSpriteFree(tower->p);
    freeSpriteXPM(tower->lowerHalf);
    freeSpriteXPM(tower->upperHalf);
    boxColliderFree(tower->hitBox);
    free(tower);
}

BoxCollider* towerGetHitBox(GenericTower* tower) {
    return tower->hitBox;
}
