#include "../include/arena.h"
#include "../include/components/boxCollider.h"
#include "../include/player/player.h"
#include "../include/components/projectile.h"
#include "../include/tiles/collectable.h"
#include "../include/tiles/tile.h"
#include "../include/utils/cursorHandler.h"

#include "../resources/1.h"
#include "../resources/tiles/tiles.h"
#include "../resources/map/background.h"

#include "../../lab5/spriteXMP.h"

#include <stdlib.h>

#define MAP_WIDTH 40 
#define MAP_HEIGHT 32 
#define TILE_SIZE 32

struct Arena {
    int tilesN;
    Tile** tiles;
    int width;
    int height;
    int tileSize;

    Player* player;
    int fruitsN;
    Collectable** fruits;

    int projectilesCapacity;
    int projectilesCount;
    Projectile** projectiles;

    Cursor* cursor;
};

static void createMap(Arena* arena, const char* filename, int tileSize) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open map file: %s\n", filename);
        return;
    }

    int width = MAP_WIDTH;
    int height = MAP_HEIGHT;
    char lineBuffer[1024];

    arena->tiles = malloc(sizeof(Tile*) * (width * height + 4));
    arena->width = width;
    arena->height = height;
    arena->tileSize = tileSize;

    int** map = malloc(height * sizeof(int*));
    for (int i = 0; i < height; i++) {
        map[i] = malloc(width * sizeof(int));

        if (fgets(lineBuffer, sizeof(lineBuffer), file) == NULL) {
            break;
        }

        char* ptr = lineBuffer;
        for (int j = 0; j < width; j++) {
            map[i][j] = atoi(ptr);
            while (*ptr != '\0' && *ptr != '\n' && *ptr != '\r' && *ptr != ',') ptr++;
            if (*ptr == ',') ptr++;
        }
    }

    fclose(file);
	int i = 0;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int id = map[y][x];
			if (id <= 0) continue;

            char** sprite = tiles[id - 1];
            arena->tiles[i] = tileNew(sprite, id, x * tileSize, y * tileSize, tileSize, tileSize);
			i++;
        }
    }


	// Top inv wall
	arena->tiles[i] = tileNew(NULL, -1, 0, -10, MAP_WIDTH * TILE_SIZE, 10);

	// Bot inv wall
	arena->tiles[i + 1] = tileNew(NULL, -1, 0, MAP_HEIGHT * TILE_SIZE, MAP_WIDTH * TILE_SIZE, 10);

	// Left inv wall
	arena->tiles[i + 2] = tileNew(NULL, -1, -10, 0, 10, MAP_HEIGHT * TILE_SIZE);

	// Right inv wall
	arena->tiles[i + 3] = tileNew(NULL, -1, MAP_WIDTH * TILE_SIZE, 0, 10, MAP_HEIGHT * TILE_SIZE);
	i += 4;

	arena->tilesN = i;
    for (int i = 0; i < height; i++) free(map[i]);
    free(map);
}

Arena* arenaNew(Terminal* terminal, const char* filename) {
    Arena* res = malloc(sizeof(Arena));
    createMap(res, filename, TILE_SIZE);

    Vec2d playerPos = {100, 100};
    res->player = playerNew(playerPos);

    res->fruitsN = 1;
    res->fruits = malloc(sizeof(Collectable*) * res->fruitsN);
    Vec2d fruitPos = {500, 400};
    Vec2d fruitOffset = {0, 0};
    Vec2d fruitSize = {32, 32};

    res->fruits[0] = collectableNew(fruitPos, fruitOffset, fruitSize, abc);

    res->projectilesCapacity = 100;
    res->projectilesCount = 0;
    res->projectiles = malloc(sizeof(Projectile*) * res->projectilesCapacity);

	// The terminal will handle the freeing
	SpriteXPM* backgroundSprite = newSpriteXPM((xpm_map_t)background, XPM_8_8_8, 0, 0);
	generateEmptyBuffer(backgroundSprite, terminal, 0, 0, false);

    SpriteXPM* arrowSprite = newSpriteXPM((xpm_map_t)abc, XPM_8_8_8, 0, 0);
    Projectile* testProj = projectileCreate(PROJ_HOMING, 200, 200, 100, 0, arrowSprite);
    arenaAddProjectile(res, testProj);

    return res;
}

void arenaFree(Arena* arena) {
    for (int i = 0; i < arena->tilesN; i++) tileFree(arena->tiles[i]);
    free(arena->tiles);
    
    for (int i = 0; i < arena->projectilesCount; i++) projectileFree(arena->projectiles[i]);
    free(arena->projectiles);

    playerFree(arena->player);
    free(arena);
}

int arenaDetectCollisionsWalls(Arena* arena, BoxCollider* box, int tag, BoxCollider** colliders) {
    int j = 0;

    if (arena->tileSize > 0 && arena->width > 0 && arena->height > 0) {
        int boxX = boxColliderGetX(box);
        int boxY = boxColliderGetY(box);
        int boxW = boxColliderGetWidth(box);
        int boxH = boxColliderGetHeight(box);

        int startX = boxX / arena->tileSize;
        int startY = boxY / arena->tileSize;
        int endX = (boxX + boxW) / arena->tileSize;
        int endY = (boxY + boxH) / arena->tileSize;

        if (startX < 0) startX = 0;
        if (startY < 0) startY = 0;
        if (endX >= arena->width) endX = arena->width - 1;
        if (endY >= arena->height) endY = arena->height - 1;

        for (int y = startY; y <= endY; y++) {
            for (int x = startX; x <= endX; x++) {
                Tile* t = arena->tiles[y * arena->width + x];
                BoxCollider* collider = tileBoxCollider(t);
                if (collider != NULL) {
                    if (tag == boxColliderGetTag(collider) && boxIsCollidingWithBox(box, collider)) {
                        colliders[j++] = collider;
                    }
                }
            }
        }
    }

    return j;
}

int arenaDetectCollisions(Arena* arena, BoxCollider* box, int tag, BoxCollider** colliders) {
    int j = 0;
    for (int i = 0; i < arena->tilesN; i++) {
        BoxCollider* collider = tileBoxCollider(arena->tiles[i]);
        if (collider == NULL) continue;

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
    for (int i = 0; i < arena->fruitsN; i++)
        if (collectableIsActive(arena->fruits[i])) colliders[i] = collectableGetHitBox(arena->fruits[i]);

    BoxCollider* collisions[arena->fruitsN];
    int n = arenaDetectCollisions2(arena, hitBox, arena->fruitsN, colliders, collisions);

    for (int i = 0; i < n; i++) {
        collectableSetActive(arena->fruits[i], false);
    }
}

void arenaAddProjectile(Arena* arena, Projectile* p) {
    if (arena->projectilesCount >= arena->projectilesCapacity) return;
    arena->projectiles[arena->projectilesCount++] = p;
}

void arenaProjectileHandler(Arena* arena, float deltaTime, Terminal* terminal) {
    int activeCount = 0;
    for (int i = 0; i < arena->projectilesCount; i++) {
        if (projectileIsActive(arena->projectiles[i])) {
            arena->projectiles[activeCount++] = arena->projectiles[i];
        } else {
            projectileFree(arena->projectiles[i]);
        }
    }
    arena->projectilesCount = activeCount;

    for (int i = 0; i < arena->projectilesCount; i++) {
        Projectile* p = arena->projectiles[i];
        projectileUpdate(p, deltaTime, terminal);
        
        BoxCollider* colliders[10];
        int n = arenaDetectCollisions(arena, projectileGetCollider(p), 0, colliders);
        if (n > 0) {
            projectileSetActive(p, false);
        }
    }
}

void arenaUpdate(Arena* arena, Terminal* terminal, float deltaTime) {
    for (int i = 0; i < arena->fruitsN; i++) collectableUpdate(arena->fruits[i], terminal);
    for (int i = 0; i < arena->tilesN; i++) tileUpdate(arena->tiles[i], terminal);

    arenaCollectableHandler(arena);
    projectileUpdateHomingTarget(arena->projectiles[0], playerModelGetX(playerGetPlayerModel(arena->player)), playerModelGetY(playerGetPlayerModel(arena->player)));
    arenaProjectileHandler(arena, deltaTime, terminal);
    playerUpdate(arena->player, terminal, arena, deltaTime);
	cursorUpdate(getCursor(), terminal);
}
