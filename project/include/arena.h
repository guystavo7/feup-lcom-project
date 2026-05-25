#ifndef ARENA
#define ARENA

#include "./components/boxCollider.h"
#include "./components/projectile.h"
#include "../../lab5/terminal.h"

#include <stdbool.h>

typedef struct Arena Arena;

Arena* arenaNew(Terminal* terminal, const char* filename);
void arenaFree(Arena* arena);

int arenaDetectCollisions(Arena* arena, BoxCollider* box, int tag, BoxCollider** colliders);

void arenaCollectableHandler(Arena* arena);
int arenaDetectCollisions2(Arena* arena, BoxCollider* hitBox, int n, BoxCollider** colliders, BoxCollider** result);
int arenaDetectCollisionsWalls(Arena* arena, BoxCollider* box, int tag, BoxCollider** colliders);

void arenaUpdate(Arena* arena, Terminal* terminal, float deltaTime);

void arenaAddProjectile(Arena* arena, Projectile* p);
void arenaProjectileHandler(Arena* arena, float deltaTime, Terminal* terminal);

#endif
