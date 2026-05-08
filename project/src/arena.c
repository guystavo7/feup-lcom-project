#include "../include/arena.h"
#include "../include/boxCollider.h"

#include <stdlib.h>

struct Arena {
	int collidersN;
	BoxCollider** walls;
};

Arena* newArena() {
	Arena* res = malloc(sizeof(Arena));
	res->walls = malloc(sizeof(BoxCollider*) * 4);
	
	BoxCollider* topWall = newBoxCollider(0, -10, 800, 10, 0);
	BoxCollider* botWall = newBoxCollider(0, 590, 800, 10, 0);
	BoxCollider* leftWall = newBoxCollider(-10, 0, 10, 600, 0);
	BoxCollider* rightWall = newBoxCollider(590, 0, 10, 600, 0);

	res->walls[0] = topWall;
	res->walls[1] = botWall;
	res->walls[2] = leftWall;
	res->walls[3] = rightWall;
	res->collidersN = 4;

	return res;
}

void freeArena(Arena *arena){
	for (int i = 0; i < arena->collidersN; i++) free(arena->walls[i]);
	free(arena->walls);
	free(arena);
}

int arenaDetectCollisions(Arena* arena, BoxCollider *box, int tag, BoxCollider **colliders) {
	int j = 0;

	for (int i = 0; i < arena->collidersN; i++) {
		BoxCollider* collider = arena->walls[i];
		if (tag != boxColliderGetTag(collider)) continue;
		if (boxIsCollidingWithBox(box, collider)) {
			colliders[j] = collider;
			j++;
		}	
	}

	return j;
}
