#include "../../include/monster/monsterGen.h"

#include "../../include/arena.h"
#include "../../include/data.h"
#include "../../include/monster/alienMonster.h"
#include "../../include/monster/bossMonster.h"
#include "../../include/monster/lancerMonster.h"
#include "../../include/monster/pawnMonster.h"

#include <stdlib.h>
#include <string.h>

Monster* monsterCreateFromKind(MonsterKind kind, Path* path) {
    Vec2d startPos;
    pathGetPoint(path, 0, &startPos);
    switch (kind) {
        case MONSTER_KIND_PAWN:
            return pawnMonsterNew(startPos.x, startPos.y, path);
        case MONSTER_KIND_ALIEN:
            return alienMonsterNew(startPos.x, startPos.y, path);
        case MONSTER_KIND_LANCER:
            return lancerMonsterNew(startPos.x, startPos.y, path);
        case MONSTER_KIND_BOSS:
            return bossMonsterNew(startPos.x, startPos.y, path);
        default:
            return pawnMonsterNew(startPos.x, startPos.y, path);
    }
}

struct monsterGen {
    Vector* monsterList;
    Vector* availableMonsterIdStack;
    Vector* deactivatedMonsterIdList;
    Vector* activeMonsterIdSorted;
    Path* path;

    unsigned int numberOfInstantFinishedMonsters;

    Vector* spawnKindQueue;
};

void freeMonsterList(void* m) {
    Monster* monster = *(Monster**)m;
    monsterFree(monster);
}

MonsterGen* monsterGenNew(Path* path) {
    MonsterGen* mg = malloc(sizeof(MonsterGen));
    mg->path = path;

    mg->monsterList = vectorCreate(sizeof(Monster*), freeMonsterList);
    mg->availableMonsterIdStack = vectorCreate(sizeof(int), NULL);
    mg->activeMonsterIdSorted = vectorCreate(sizeof(int), NULL);
    mg->deactivatedMonsterIdList = vectorCreate(sizeof(bool), NULL);
    mg->numberOfInstantFinishedMonsters = 0;

    mg->spawnKindQueue = vectorCreate(sizeof(MonsterKind), NULL);

    return mg;
}

void monsterGenFree(MonsterGen* mg) {
    vectorFree(mg->monsterList);
    vectorFree(mg->availableMonsterIdStack);
    vectorFree(mg->deactivatedMonsterIdList);
    vectorFree(mg->activeMonsterIdSorted);
    vectorFree(mg->spawnKindQueue);
    free(mg);
}

static void monsterGenSpawnOneOfKind(MonsterGen* mg, MonsterKind kind) {
    int stackSize = (int)vectorGetSize(mg->availableMonsterIdStack);
    int monsterId;

    if (stackSize > 0) {
        monsterId = (int)vectorGetVal(mg->availableMonsterIdStack, (unsigned int)(stackSize - 1));
        Monster** slot = (Monster**)vectorGet(mg->monsterList, (unsigned int)monsterId);
        monsterFree(*slot);
        *slot = monsterCreateFromKind(kind, mg->path);
        vectorPopBack(mg->availableMonsterIdStack);
        vectorSetVal(mg->deactivatedMonsterIdList, (unsigned int)monsterId, false);
        vectorPushBackVal(mg->activeMonsterIdSorted, (long long)monsterId);
    } else {
        Monster* new_monster = monsterCreateFromKind(kind, mg->path);
        vectorPushBack(mg->monsterList, (char*)&new_monster);
        vectorPushBackVal(mg->deactivatedMonsterIdList, false);
        vectorPushBackVal(mg->activeMonsterIdSorted, (long long)(vectorGetSize(mg->monsterList) - 1));
    }
}

void monsterGenQueueSpawns(MonsterGen* mg, MonsterKind kind, int count) {
    int i;
    if (mg == NULL || count <= 0) {
        return;
    }
    for (i = 0; i < count; i++) {
        MonsterKind k = kind;
        vectorPushBack(mg->spawnKindQueue, (char*)&k);
    }
}

void monsterGenUpdate(MonsterGen* mg, Arena* arena, Terminal* terminal, float delta_t) {
    unsigned int drain_budget = (unsigned int)MONSTER_GEN_SPAWN_DRAIN_PER_FRAME;

    while (vectorGetSize(mg->spawnKindQueue) > 0 && drain_budget > 0) {
        MonsterKind kind;
        memcpy(&kind, vectorGet(mg->spawnKindQueue, 0), sizeof(MonsterKind));
        vectorRemove(mg->spawnKindQueue, 0);
        monsterGenSpawnOneOfKind(mg, kind);
        drain_budget--;
    }

    mg->numberOfInstantFinishedMonsters = 0;

    for (unsigned int i = 0; i < vectorGetSize(mg->monsterList); i++) {
        Monster* m = *(Monster**)vectorGet(mg->monsterList, i);
        monsterUpdate(m, arena, terminal, delta_t);
        bool m_in_stack = vectorGetVal(mg->deactivatedMonsterIdList, i);

        if ((monsterIsDead(m) || monsterGetFinishedPath(m)) && !m_in_stack) {
            vectorSetVal(mg->deactivatedMonsterIdList, i, true);
            vectorPushBack(mg->availableMonsterIdStack, (char*)&i);

            if (!(monsterIsDead(m))) {
                mg->numberOfInstantFinishedMonsters++;
            }

            if (arena != NULL && monsterIsDead(m)) {
                BoxCollider* bc = monsterGetBoxCollider(m);
                float ax = boxColliderGetX(bc) + boxColliderGetOffsetX(bc);
                float ay = boxColliderGetY(bc) + boxColliderGetOffsetY(bc);
                Vec2d pos = {ax + boxColliderGetWidth(bc) * 0.5f - FRUIT_SIZE.x * 0.5f,
                             ay + boxColliderGetHeight(bc) * 0.5f - FRUIT_SIZE.y * 0.5f};
                arenaDeathFruit(arena, pos);
            }
        }
    }

    unsigned int new_size = 0;
    for (unsigned int i = 0; i < vectorGetSize(mg->activeMonsterIdSorted); i++) {
        int monsterId = (int)vectorGetVal(mg->activeMonsterIdSorted, i);
        if (!vectorGetVal(mg->deactivatedMonsterIdList, (unsigned int)monsterId)) {
            vectorSetVal(mg->activeMonsterIdSorted, new_size, (long long)monsterId);
            new_size++;
        }
    }
    unsigned int n_removes = vectorGetSize(mg->activeMonsterIdSorted) - new_size;

    for (unsigned int i = 0; i < n_removes; i++) {
        vectorPopBack(mg->activeMonsterIdSorted);
    }
}

int monsterGenGetNextActiveMonsterIndex(MonsterGen* mg, unsigned int sortedIndex) {
    if (vectorGetSize(mg->activeMonsterIdSorted) == 0 || sortedIndex >= vectorGetSize(mg->activeMonsterIdSorted)) {
        return -1;
    }
    return (int)vectorGetVal(mg->activeMonsterIdSorted, sortedIndex);
}

Monster* monsterGenGetMonster(MonsterGen* mg, unsigned int index) {
    return *(Monster**)vectorGet(mg->monsterList, index);
}

unsigned int monsterGenGetNumberOfInstantFinishedMonsters(MonsterGen* mg) {
    return mg->numberOfInstantFinishedMonsters;
}
