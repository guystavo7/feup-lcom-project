#pragma once

#include "monster.h"
#include "monsterKind.h"

typedef struct monsterGen MonsterGen;

Monster* monsterCreateFromKind(MonsterKind kind, Path* path);

void freeMonsterList(void* m);

MonsterGen* monsterGenNew(Path* path);

void monsterGenFree(MonsterGen* mg);

void monsterGenQueueSpawns(MonsterGen* mg, MonsterKind kind, int count);

void monsterGenUpdate(MonsterGen* mg, Arena* arena, Terminal* terminal, float delta_t);

int monsterGenGetNextActiveMonsterIndex(MonsterGen* mg, unsigned int sortedIndex);
Monster* monsterGenGetMonster(MonsterGen* mg, unsigned int index);

unsigned int monsterGenGetNumberOfInstantFinishedMonsters(MonsterGen* mg);
