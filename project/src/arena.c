#include "../include/arena.h"
#include "../include/boxCollider.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct Arena {
    int collidersN;
    BoxCollider** walls;
};

Arena* arenaNewFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open map file: %s\n", filename);
        return NULL;
    }

    int width, height;
    if (fscanf(file, "%d %d", &width, &height) != 2) {
        printf("Error: Invalid map file format\n");
        fclose(file);
        return NULL;
    }

    char** map = malloc(height * sizeof(char*));
    for (int i = 0; i < height; i++) {
        map[i] = malloc(width + 1);
        if (fscanf(file, "%s", map[i]) != 1) {
            printf("Error: Could not read map row %d\n", i);
            fclose(file);
            return NULL;
        }
    }
    fclose(file);

    Arena* res = malloc(sizeof(Arena));
    res->walls = malloc(sizeof(BoxCollider*) * (width * height));
    
    int colliderIdx = 0;
    int tileSize = 10;
    Vec2d offset = {0, 0};

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width && map[y][x] != '\0'; x++) {
            if (map[y][x] == '#') {
                Vec2d wallPos = {x * tileSize, y * tileSize};
                Vec2d wallSize = {tileSize, tileSize};
                res->walls[colliderIdx++] = boxColliderNew(wallPos, offset, wallSize, 0);
            }
        }
    }

    res->collidersN = colliderIdx;

    for (int i = 0; i < height; i++) free(map[i]);
    free(map);

    return res;
}

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

    return res;
}

void arenaFree(Arena* arena) {
    for (int i = 0; i < arena->collidersN; i++) free(arena->walls[i]);
    free(arena->walls);
    free(arena);
}

int arenaDetectCollisions(Arena* arena, BoxCollider* box, int tag, BoxCollider** colliders) {
    int j = 0;

    for (int i = 0; i < arena->collidersN; i++) {
        BoxCollider* collider = arena->walls[i];
        if (tag != boxColliderGetTag(collider)) continue;
        if (boxIsCollidingWithBox(box, collider)) {
			printf("oi\n");
            colliders[j] = collider;
            j++;
        }
    }

    return j;
}
