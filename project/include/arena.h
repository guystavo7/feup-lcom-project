#ifndef ARENA
#define ARENA

#include "boxCollider.h"
#include <stdbool.h>
#include "../../lab5/terminal.h"

typedef struct Arena Arena;

Arena* arenaNewFromFile(const char* filename, int tileSize);

void arenaFree(Arena* arena);

int arenaDetectCollisions(Arena* arena, BoxCollider* box, int tag, BoxCollider** colliders);

void arenaUpdade(Arena* arena, Terminal* terminal);

#endif
