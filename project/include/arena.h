#ifndef ARENA
#define ARENA

#include "./components/boxCollider.h"
#include "./components/projectile.h"
#include "../../lab5/terminal.h"
#include "data_structures/vector.h"
#include "utils/vec2d.h"

#include <stdbool.h>

typedef struct Arena Arena;


Arena* arenaNew(Terminal* terminal, const char* tilesFile, const char* pathFile, const char* waterFile, char** background, Vector* paths, Vector* gens, int bananaStart);
void arenaFree(Arena* arena);

int arenaDetectCollisions(Arena* arena, BoxCollider* box, int tag, BoxCollider** colliders);
BoxCollider* arenaFindFirstEnemy(Arena* arena, BoxCollider* range);

void arenaUpdate(Arena* arena, Terminal* terminal, float deltaTime);
void arenaBuildTower(Arena* arena, Vec2d position);

bool arenaCanPlaceTower(Arena* arena, Vec2d position);
void arenaDeathFruit(Arena* arena, Vec2d position);
bool arenaIsGameOver(Arena* arena, int* health);

#endif
