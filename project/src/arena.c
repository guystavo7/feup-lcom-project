#include "../include/arena.h"
#include "../include/boxCollider.h"
#include "../include/tile.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

    int width, height;
    if (fscanf(file, "%d %d", &width, &height) != 2) {
        printf("Error: Invalid map file format\n");
        fclose(file);
        return NULL;
    }

    char** map = malloc(height * sizeof(char*));
    for (int i = 0; i < height; i++) {
        map[i] = malloc(width + 1);
        for (int j = 0; j < width; j++) {
            int c;
            do {
                c = fgetc(file);
            } while (c == '\n' || c == '\r');
            
            if (c == EOF) {
                map[i][j] = '\0';
                break;
            }
            map[i][j] = (char)c;
        }
        map[i][width] = '\0';
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
            if (map[y][x] == '#') {
                res->tiles[y * width + x] = tileNew("#", "wall", x * tileSize, y * tileSize, tileSize);
            } else {
                res->tiles[y * width + x] = tileNew(" ", "empty", x * tileSize, y * tileSize, tileSize);
            }
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
