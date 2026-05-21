#include "../include/arena.h"
#include "../include/boxCollider.h"
#include "../include/tile.h"
#include <stdlib.h>
#include <stdio.h>
#include "../resources/tiles/tiles.h"
#include <string.h>

#define MAP_WIDTH 25
#define MAP_HEIGHT 18

struct Arena {
    int tilesN;
    Tile** tiles;
    int w;
    int h;
    int tileSize;
};

Arena* arenaNewFromFile(const char* filename, int tileSize) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open map file: %s\n", filename);
        return NULL;
    }

    int width = MAP_WIDTH;
    int height = MAP_HEIGHT;
    char lineBuffer[1024];

    int** map = malloc(height * sizeof(int*));
    for (int i = 0; i < height; i++) {
        map[i] = malloc(width * sizeof(int));
        
        if (fgets(lineBuffer, sizeof(lineBuffer), file) == NULL) {
            break;
        }

        char* ptr = lineBuffer;
        for (int j = 0; j < width; j++) {
            map[i][j] = atoi(ptr);
            while (*ptr != '\0' && *ptr != '\n' && *ptr != '\r' && *ptr != ',') {
                ptr++;
            }
            if (*ptr == ',') ptr++;
        }
    }
    fclose(file);

    Arena* res = malloc(sizeof(Arena));
    res->tiles = malloc(sizeof(Tile*) * (width * height));
    res->w = width;
    res->h = height;
    res->tileSize = tileSize;
    res->tilesN = width * height;
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int id = map[y][x];
            char** sprite = NULL;
            int num_tiles = sizeof(tiles) / sizeof(tiles[0]);
            
            if (id > 0 && id <= num_tiles) {
                sprite = tiles[id - 1];
            }
            res->tiles[y * width + x] = tileNew(sprite, id, x * tileSize, y * tileSize, tileSize);
        }
    }

    for (int i = 0; i < height; i++) free(map[i]);
    free(map);

    return res;
}

void arenaFree(Arena* arena) {
    for (int i = 0; i < arena->tilesN; i++) tileFree(arena->tiles[i]);
    free(arena->tiles);
    free(arena);
}

int arenaDetectCollisions(Arena* arena, BoxCollider* box, int tag, BoxCollider** colliders) {
    int j = 0;

    if (arena->tileSize > 0 && arena->w > 0 && arena->h > 0) {
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
        if (endX >= arena->w) endX = arena->w - 1;
        if (endY >= arena->h) endY = arena->h - 1;

        for (int y = startY; y <= endY; y++) {
            for (int x = startX; x <= endX; x++) {
                Tile* t = arena->tiles[y * arena->w + x];
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

void arenaUpdade(Arena* arena, Terminal* terminal) {
    for (int i = 0; i < arena->tilesN; i++) {
        tileUpdate(arena->tiles[i], terminal);
    }
}
