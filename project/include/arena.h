#ifndef ARENA
#define ARENA

#include "./components/boxCollider.h"
#include "./components/projectile.h"
#include "../../lab5/terminal.h"
#include "utils/vec2d.h"

#include <stdbool.h>

#define WALLS_TAG 		0
#define PLAYER_TAG 		1
#define MONSTER_TAG		3
#define COLLECTABLE_TAG	4
#define UI_TAG			5
#define TAGS_NUM 		5

typedef struct Arena Arena;

Arena* arenaNew(Terminal* terminal, const char* filename);
void arenaFree(Arena* arena);

int arenaDetectCollisions(Arena* arena, BoxCollider* box, int tag, BoxCollider** colliders);

void arenaUpdate(Arena* arena, Terminal* terminal, float deltaTime);
void arenaBuildTower(Arena* arena, Vec2d position);

void arenaAddProjectile(Arena* arena, Projectile* p);
void arenaProjectileHandler(Arena* arena, float deltaTime, Terminal* terminal);

#endif
