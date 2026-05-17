#ifndef ARENA
#define ARENA

#include "boxCollider.h"
#include <stdbool.h>

typedef struct Arena Arena;

Arena* arenaNew();
void arenaFree(Arena* arena);

int arenaDetectCollisions(Arena* arena, BoxCollider* box, int tag, BoxCollider** colliders);

#endif
