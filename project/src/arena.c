#include "../include/arena.h"
#include "../include/components/boxCollider.h"
#include "../include/player/player.h"
#include "../include/tiles/collectable.h"
#include "../include/tiles/tile.h"
#include "../include/utils/cursorHandler.h"
#include "../include/buildings/basicTower.h"
#include "../include/buildings/genericTower.h"

#include "../resources/1.h"
#include "../resources/tiles/tiles.h"
#include "../resources/map/background.h"

#include "../../lab5/spriteXMP.h"

#include <stdio.h>
#include <stdlib.h>

#define MAP_WIDTH 40 
#define MAP_HEIGHT 32 
#define TILE_SIZE 32

struct Arena {
	// 0 - walls, 1 - player, 2 - monsters, 3 - collectables, 4 - projectiles, 5 - UI
	int* collidersN;
	BoxCollider*** colliders;

    int tilesN;
    Tile** tiles;

    int fruitsN;
    Collectable** fruits;

	int towersN;
	GenericTower** towers;

    int width;
    int height;
    int tileSize;

    Player* player;
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


	arena->tiles[i] = tileNew(NULL, -1, 0, -10, MAP_WIDTH * TILE_SIZE, 10);
	arena->tiles[i + 1] = tileNew(NULL, -1, 0, MAP_HEIGHT * TILE_SIZE, MAP_WIDTH * TILE_SIZE, 10);
	arena->tiles[i + 2] = tileNew(NULL, -1, -10, 0, 10, MAP_HEIGHT * TILE_SIZE);
	arena->tiles[i + 3] = tileNew(NULL, -1, MAP_WIDTH * TILE_SIZE, 0, 10, MAP_HEIGHT * TILE_SIZE);
	i += 4;
	arena->tilesN = i;

	arena->collidersN[WALLS_TAG] = arena->tilesN;
	arena->colliders[WALLS_TAG] = malloc(sizeof(BoxCollider*) * (i + 4));
    for (int i = 0; i < height; i++) free(map[i]);
	for (int j = 0; j < arena->tilesN; j++) arena->colliders[WALLS_TAG][j] = tileBoxCollider(arena->tiles[j]);

    free(map);
}

static void createPlayer(Arena* arena) {
    Vec2d playerPos = {100, 100};
    arena->player = playerNew(playerPos);
	
	arena->colliders[PLAYER_TAG] = malloc(sizeof(BoxCollider*));
	arena->colliders[PLAYER_TAG][0] = playerModelGetHitBox(playerGetPlayerModel(arena->player));
	arena->collidersN[PLAYER_TAG] = 1;
}

static void createFruitsTest(Arena* arena) {
    arena->fruitsN = 1;
    arena->fruits = malloc(sizeof(Collectable*) * arena->fruitsN);
    Vec2d fruitPos = {500, 400};
    Vec2d fruitOffset = {0, 0};
    Vec2d fruitSize = {32, 32};

	arena->fruits[0] = collectableNew(fruitPos, fruitOffset, fruitSize, abc);
	arena->colliders[COLLECTABLE_TAG] = malloc(sizeof(BoxCollider*) * arena->fruitsN);
	arena->collidersN[COLLECTABLE_TAG] = arena->fruitsN;
	for (int i = 0; i < arena->fruitsN; i++) arena->colliders[COLLECTABLE_TAG][i] = collectableGetHitBox(arena->fruits[i]);
}

Arena* arenaNew(Terminal* terminal, const char* filename) {
    Arena* res = malloc(sizeof(Arena));
	res->collidersN = malloc(sizeof(int) * TAGS_NUM);
	res->colliders = malloc(sizeof(BoxCollider*) * TAGS_NUM);

    createMap(res, filename, TILE_SIZE);
	createPlayer(res);
	createFruitsTest(res);

	res->towersN = 1;
	res->towers = malloc(sizeof(GenericTower*));
	Vec2d pos = {300, 400};
	res->towers[0] = basicTowerNew(pos);

	res->collidersN[WALLS_TAG]++;
	res->colliders[WALLS_TAG] = realloc(res->colliders[WALLS_TAG], sizeof(BoxCollider*) * res->collidersN[WALLS_TAG]);
	res->colliders[WALLS_TAG][res->collidersN[WALLS_TAG] - 1] = towerGetHitBox(res->towers[0]);

	// The terminal will handle the freeing
	SpriteXPM* backgroundSprite = newSpriteXPM((xpm_map_t)background, XPM_8_8_8, 0, 0);
	generateEmptyBuffer(backgroundSprite, terminal, 0, 0, false);

    return res;
}

void arenaFree(Arena* arena) {
    for (int i = 0; i < arena->tilesN; i++) tileFree(arena->tiles[i]);
	for (int i = 0; i < arena->fruitsN; i++) collectableFree(arena->fruits[i]);
	for (int i = 0; i < TAGS_NUM; i++) free(arena->colliders);
	free(arena->collidersN);
    free(arena->tiles);
	free(arena->fruits);
    playerFree(arena->player);
    free(arena);
}

int arenaDetectCollisions(Arena* arena, BoxCollider* box, int tag, BoxCollider** colliders) {
    int j = 0;
	BoxCollider** c = arena->colliders[tag];
	int n = arena->collidersN[tag];
	
    for (int i = 0; i < n; i++) {
        BoxCollider* collider = c[i]; 
        if (collider == NULL) continue;
        if (boxIsCollidingWithBox(box, collider)) {
            colliders[j] = collider;
            j++;
        }
    }

    return j;
}

void arenaCollectableHandler(Arena* arena) {
    BoxCollider* hitBox = playerModelGetHitBox(playerGetPlayerModel(arena->player));
    BoxCollider* collisions[arena->fruitsN];
    int n = arenaDetectCollisions(arena, hitBox, COLLECTABLE_TAG, collisions);
    for (int i = 0; i < n; i++) collectableSetActive(arena->fruits[i], false);
}

void arenaUpdate(Arena* arena, Terminal* terminal, float deltaTime) {
    for (int i = 0; i < arena->fruitsN; i++) collectableUpdate(arena->fruits[i], terminal);
    for (int i = 0; i < arena->tilesN; i++) tileUpdate(arena->tiles[i], terminal);
	for (int i = 0; i < arena->towersN; i++) towerUpdate(arena->towers[i], terminal, arena, deltaTime);

    arenaCollectableHandler(arena);
    playerUpdate(arena->player, terminal, arena, deltaTime);
	cursorUpdate(getCursor(), terminal, deltaTime);
}
