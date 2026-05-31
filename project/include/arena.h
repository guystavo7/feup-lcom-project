#ifndef ARENA
#define ARENA

#include "./components/boxCollider.h"
#include "../../lab5/terminal.h"
#include "utils/vec2d.h"

#include <stdbool.h>

typedef struct Arena Arena;

Arena* arenaNew(Terminal* terminal, const char* filename);
void arenaFree(Arena* arena);

int arenaDetectCollisions(Arena* arena, BoxCollider* box, int tag, BoxCollider** colliders);

void arenaUpdate(Arena* arena, Terminal* terminal, float deltaTime);
void arenaBuildTower(Arena* arena, Vec2d position);

#endif
