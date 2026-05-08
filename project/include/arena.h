#ifndef ARENA
#define ARENA

#include "boxCollider.h"
#include <stdbool.h>

typedef struct Arena Arena;

Arena* newArena();
void freeArena(Arena* arena);

int arenaDetectCollisions(Arena* arena, BoxCollider* box, int tag, BoxCollider** colliders);

#endif
