#ifndef ARENA
#define ARENA

#include "./components/boxCollider.h"
#include "../../lab5/terminal.h"
#include <stdbool.h>

typedef struct Arena Arena;

Arena* arenaNew();
void arenaFree(Arena* arena);

int arenaDetectCollisions(Arena* arena, BoxCollider* box, int tag, BoxCollider** colliders);
void arenaUpdate(Arena* arena, Terminal* terminal, float deltaTime);

#endif
