#include "../include/arena.h"
#include "../include/components/boxCollider.h"
#include "../include/player/player.h"
#include "../include/tiles/collectable.h"

#include "../resources/1.h"
#include <stdlib.h>

struct Arena {
    int collidersN;
    BoxCollider** walls;

    Player* player;

    int fruitsN;
    Collectable** fruits;
};

Arena* arenaNew() {
    Arena* res = malloc(sizeof(Arena));
    res->walls = malloc(sizeof(BoxCollider*) * 4);

    Vec2d verticalWallSize = {10, 600};
    Vec2d horizontalWallSize = {800, 10};

    Vec2d topWallPos = {0, -10};
    Vec2d botWallPos = {0, 600};
    Vec2d leftWallPos = {-10, 0};
    Vec2d rightWallPos = {800, 0};

    Vec2d offset = {0, 0};

    BoxCollider* topWall = boxColliderNew(topWallPos, offset, horizontalWallSize, 0);
    BoxCollider* botWall = boxColliderNew(botWallPos, offset, horizontalWallSize, 0);
    BoxCollider* leftWall = boxColliderNew(leftWallPos, offset, verticalWallSize, 0);
    BoxCollider* rightWall = boxColliderNew(rightWallPos, offset, verticalWallSize, 0);

    res->walls[0] = topWall;
    res->walls[1] = botWall;
    res->walls[2] = leftWall;
    res->walls[3] = rightWall;
    res->collidersN = 4;

    Vec2d playerPos = {100, 100};
    res->player = playerNew(playerPos);

    res->fruitsN = 1;
    res->fruits = malloc(sizeof(Collectable*) * res->fruitsN);
    Vec2d fruitPos = {500, 400};
    Vec2d fruitOffset = {0, 0};
    Vec2d fruitSize = {32, 32};

    res->fruits[0] = collectableNew(fruitPos, fruitOffset, fruitSize, abc);
    return res;
}

void arenaFree(Arena* arena) {
    for (int i = 0; i < arena->collidersN; i++) free(arena->walls[i]);
    free(arena->walls);
    playerFree(arena->player);
    free(arena);
}

int arenaDetectCollisions(Arena* arena, BoxCollider* box, int tag, BoxCollider** colliders) {
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

int arenaDetectCollisions2(Arena* arena, BoxCollider* hitBox, int n, BoxCollider** colliders, BoxCollider** result) {
    int j = 0;

    for (int i = 0; i < n; i++) {
        BoxCollider* collider = colliders[i];
        if (boxIsCollidingWithBox(hitBox, collider)) {
            result[j] = collider;
            j++;
        }
    }

    return j;
}

void arenaCollectableHandler(Arena* arena) {
    BoxCollider* hitBox = playerModelGetHitBox(playerGetPlayerModel(arena->player));
    BoxCollider* colliders[arena->fruitsN];
    for (int i = 0; i < arena->fruitsN; i++) if (collectableIsActive(arena->fruits[i])) colliders[i] = collectableGetHitBox(arena->fruits[i]);

    BoxCollider* collisions[arena->fruitsN];
    int n = arenaDetectCollisions2(arena, hitBox, arena->fruitsN, colliders, collisions);

    for (int i = 0; i < n; i++) {
		collectableSetActive(arena->fruits[i], false);
	}
}

void arenaUpdate(Arena* arena, Terminal* terminal, float deltaTime) {
    playerUpdate(arena->player, terminal, arena, deltaTime);
    for (int i = 0; i < arena->fruitsN; i++) collectableUpdate(arena->fruits[i], terminal);

	arenaCollectableHandler(arena);
}
