#include "../include/buildings/genericTower.h"
#include "../../lab5/spriteXMP.h"
#include "../include/components/boxCollider.h"
#include "../include/components/particle.h"

#include "../resources/particles/dust/dust.h"

#include <stdlib.h>

struct GenericTower {
    SpriteXPM* sprite;
	BoxCollider* hitBox;
	Vec2d position;
	Particle* p;

	void (*towerUpdate)(GenericTower* tower, Terminal* terminal, Arena* arena, float deltaTime);
};

GenericTower* towerNew(char** XPM, Vec2d position, Vec2d size, Vec2d hitBoxOffset, void (*towerUpdate)(GenericTower* tower, Terminal* terminal, Arena* arena, float deltaTime)) {
	GenericTower* res = malloc(sizeof(GenericTower));
	res->sprite = newSpriteXPM((xpm_map_t)XPM, XPM_8_8_8, 0, 0);

	SpriteXPM** sprites = malloc(sizeof(SpriteXPM*) * 9);
	for (int i = 0; i < 9; i++) sprites[i] = newSpriteXPM((xpm_map_t)dustXPM[i], XPM_8_8_8, 0, hitBoxOffset.y);
	res->p = particleNew(10, sprites, 9);
	particleStart(res->p);

	res->hitBox = boxColliderNew(position, hitBoxOffset, size, 0);
	res->position = position;

	res->towerUpdate = towerUpdate;
	return res;
}

void towerUpdate(GenericTower* tower, Terminal* terminal, Arena* arena, float deltaTime) {
	drawSpriteXPM(tower->sprite, terminal, tower->position.x, tower->position.y, false);
	tower->towerUpdate(tower, terminal, arena, deltaTime);
	particleUpdate(tower->position.x, tower->position.y, tower->p, terminal, deltaTime, false);
}

void towerFree(GenericTower *tower) {
	particleFree(tower->p);
	freeSpriteXPM(tower->sprite);
	boxColliderFree(tower->hitBox);
	free(tower);
}

BoxCollider* towerGetHitBox(GenericTower* tower) {
	return tower->hitBox;
}
