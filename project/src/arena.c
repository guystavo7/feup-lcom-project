#include "../include/arena.h"
#include "../include/buildings/basicTower.h"
#include "../include/buildings/genericTower.h"
#include "../include/components/boxCollider.h"
#include "../include/data.h"
#include "../include/player/player.h"
#include "../include/tiles/collectable.h"
#include "../include/tiles/tile.h"
#include "../include/utils/cursorHandler.h"

#include "../resources/1.h"
#include "../resources/map/background.h"
#include "../resources/tiles/tiles.h"

#include "../../lab5/spriteXMP.h"

#include <stdio.h>
#include <stdlib.h>

struct Arena {
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

    char lineBuffer[1024];

    arena->tiles = malloc(sizeof(Tile*) * (MAP_TILES_X * MAP_TILES_Y + 4));
    arena->width = MAP_TILES_X;
    arena->height = MAP_TILES_Y;
    arena->tileSize = tileSize;

    int** map = malloc(arena->height * sizeof(int*));
    for (int i = 0; i < arena->height; i++) {
        map[i] = malloc(arena->width * sizeof(int));

        if (fgets(lineBuffer, sizeof(lineBuffer), file) == NULL) {
            break;
        }

        char* ptr = lineBuffer;
        for (int j = 0; j < arena->width; j++) {
            map[i][j] = atoi(ptr);
            while (*ptr != '\0' && *ptr != '\n' && *ptr != '\r' && *ptr != ',') ptr++;
            if (*ptr == ',') ptr++;
        }
    }

    fclose(file);
    int i = 0;

    for (int y = 0; y < arena->height; y++) {
        for (int x = 0; x < arena->width; x++) {
            int id = map[y][x];
            if (id <= 0) continue;

            char** sprite = tiles[id - 1];
            arena->tiles[i] = tileNew(sprite, id, x * tileSize, y * tileSize, tileSize, tileSize);
            i++;
        }
    }

    arena->tiles[i] = tileNew(NULL, -1, 0, -10, MAP_TILES_X * TILE_SIZE, 10);
    arena->tiles[i + 1] = tileNew(NULL, -1, 0, MAP_TILES_Y * TILE_SIZE, MAP_TILES_X * TILE_SIZE, 10);
    arena->tiles[i + 2] = tileNew(NULL, -1, -10, 0, 10, MAP_TILES_Y * TILE_SIZE);
    arena->tiles[i + 3] = tileNew(NULL, -1, MAP_TILES_X * TILE_SIZE, 0, 10, MAP_TILES_Y * TILE_SIZE);
    i += 4;
    arena->tilesN = i;

    arena->collidersN[WALLS_TAG] = arena->tilesN;
    arena->colliders[WALLS_TAG] = malloc(sizeof(BoxCollider*) * (i + 4));
    for (int i = 0; i < arena->height; i++) free(map[i]);
    for (int j = 0; j < arena->tilesN; j++) arena->colliders[WALLS_TAG][j] = tileBoxCollider(arena->tiles[j]);

    free(map);
}

static void createMonsterPathColliders(Arena* arena, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open map file: %s\n", filename);
        return;
    }

    char lineBuffer[1024];
    int** map = malloc(arena->height * sizeof(int*));
	int num = 0;

    for (int i = 0; i < arena->height; i++) {
        map[i] = malloc(arena->width * sizeof(int));

        if (fgets(lineBuffer, sizeof(lineBuffer), file) == NULL) {
            break;
        }

        char* ptr = lineBuffer;
        for (int j = 0; j < arena->width; j++) {
            map[i][j] = atoi(ptr);
			if (map[i][j] != -1) num++;

            while (*ptr != '\0' && *ptr != '\n' && *ptr != '\r' && *ptr != ',') ptr++;
            if (*ptr == ',') ptr++;
        }
    }

    fclose(file);
    arena->collidersN[PATH_TAG] = num; 
    arena->colliders[PATH_TAG] = malloc(sizeof(BoxCollider*) * num);
	int i = 0;

    for (int y = 0; y < arena->height; y++) {
        for (int x = 0; x < arena->width; x++) {
            int id = map[y][x];
            if (id <= 0) continue;

			Vec2d position = {x * TILE_SIZE, y * TILE_SIZE};
			Vec2d size = {TILE_SIZE, TILE_SIZE};
            arena->colliders[PATH_TAG][i] = boxColliderNew(position, EMPTY_VEC2D, size, PATH_TAG);
			i++;
        }
    }

    for (int i = 0; i < arena->height; i++) free(map[i]);
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

    arena->fruits[0] = collectableNew(fruitPos, EMPTY_VEC2D, FRUIT_SIZE, abc);
    arena->colliders[COLLECTABLE_TAG] = malloc(sizeof(BoxCollider*) * arena->fruitsN);
    arena->collidersN[COLLECTABLE_TAG] = arena->fruitsN;
    for (int i = 0; i < arena->fruitsN; i++) arena->colliders[COLLECTABLE_TAG][i] = collectableGetHitBox(arena->fruits[i]);
}

Arena* arenaNew(Terminal* terminal, const char* tilesFile, const char* pathFile) {
    Arena* res = malloc(sizeof(Arena));
    res->collidersN = malloc(sizeof(int) * TAGS_NUM);
    res->colliders = malloc(sizeof(BoxCollider*) * TAGS_NUM);

    createMap(res, tilesFile, TILE_SIZE);
	createMonsterPathColliders(res, pathFile);
    createPlayer(res);
    createFruitsTest(res);
    res->towersN = 0;

    // The terminal will handle the freeing
    SpriteXPM* backgroundSprite = newSpriteXPM((xpm_map_t)background, XPM_8_8_8, 0, 0);
    generateEmptyBuffer(backgroundSprite, terminal, 0, 0, false);

    return res;
}

void arenaBuildTower(Arena* arena, Vec2d position) {
    arena->towersN++;
    arena->towers = realloc(arena->towers, sizeof(GenericTower*) * arena->towersN);
    arena->towers[arena->towersN - 1] = basicTowerNew(position);

    arena->collidersN[WALLS_TAG]++;
    arena->colliders[WALLS_TAG] = realloc(arena->colliders[WALLS_TAG], sizeof(BoxCollider*) * arena->collidersN[WALLS_TAG]);
    arena->colliders[WALLS_TAG][arena->collidersN[WALLS_TAG] - 1] = towerGetHitBox(arena->towers[arena->towersN - 1]);
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

static void arenaCollectableHandler(Arena* arena) {
    BoxCollider* hitBox = playerModelGetHitBox(playerGetPlayerModel(arena->player));
    BoxCollider* collisions[arena->fruitsN];
    int n = arenaDetectCollisions(arena, hitBox, COLLECTABLE_TAG, collisions);
    for (int i = 0; i < n; i++) collectableSetActive(arena->fruits[i], false);
}

static void arenaMouseObserver(Arena* arena) {
    if (cursorIsRightButtonPressedDown(getCursor())) {
        Vec2d position = {cursorGetX(getCursor()), cursorGetY(getCursor())};
        arenaBuildTower(arena, position);
    }
}

void arenaUpdate(Arena* arena, Terminal* terminal, float deltaTime) {
    for (int i = 0; i < arena->fruitsN; i++) collectableUpdate(arena->fruits[i], terminal);
    for (int i = 0; i < arena->tilesN; i++) tileUpdate(arena->tiles[i], terminal);
    for (int i = arena->towersN - 1; i >= 0; i--) towerUpdate(arena->towers[i], terminal, arena, deltaTime);

    arenaCollectableHandler(arena);
    arenaMouseObserver(arena);

    playerUpdate(arena->player, terminal, arena, deltaTime);
    for (int i = arena->towersN - 1; i >= 0; i--) towerDrawUpperHalf(arena->towers[i], terminal);

    cursorUpdate(getCursor(), terminal, deltaTime);
}
