#include "../include/buildings/genericTower.h"
#include "../../lab5/spriteXMP.h"
#include "../include/components/boxCollider.h"

#include <stdlib.h>

struct GenericTower {
    SpriteXPM* sprite;
	BoxCollider* hitBox;
	Vec2d position;

	void (*towerUpdate)(GenericTower* tower, Terminal* terminal, Arena* arena, float deltaTime);
};

GenericTower* towerNew(char** XPM, Vec2d position, Vec2d size, Vec2d hitBoxOffset, void (*towerUpdate)(GenericTower* tower, Terminal* terminal, Arena* arena, float deltaTime)) {
	GenericTower* res = malloc(sizeof(GenericTower));
	res->sprite = newSpriteXPM((xpm_map_t)XPM, XPM_8_8_8, 0, 0);

	res->hitBox = boxColliderNew(position, hitBoxOffset, size, 0);
	res->position = position;
	res->towerUpdate = towerUpdate;
	return res;
}

void towerUpdate(GenericTower* tower, Terminal* terminal, Arena* arena, float deltaTime) {
	drawSpriteXPM(tower->sprite, terminal, tower->position.x, tower->position.y, false);
	tower->towerUpdate(tower, terminal, arena, deltaTime);
}

void towerFree(GenericTower *tower) {
	freeSpriteXPM(tower->sprite);
	boxColliderFree(tower->hitBox);
	free(tower);
}

BoxCollider* towerGetHitBox(GenericTower* tower) {
	return tower->hitBox;
}
