#include "../../include/monster/monsterGen.h"

#include "../../include/arena.h"
#include "../../include/data.h"

#include <stdlib.h>

Monster* defaultMonsterCreator(Path* path) {
    Vec2d startPos;
    pathGetPoint(path,0,&startPos);

    return alienMonsterNew(startPos.x,startPos.y, path);
}

Monster* pawnMonsterCreator(Path* path) {
    Vec2d startPos;
    pathGetPoint(path,0,&startPos);

    return pawnMonsterNew(startPos.x,startPos.y, path);
}
 
int constantSlowGenTimeFunc(float totalTimeElapsed,Timer* genTimer) { 


    timerSetInitialTime(genTimer,4); 
    
    if(totalTimeElapsed<15 ){
        timerRestart(genTimer);;  
    }
    int res = 0;
    if (!timerIsRunning(genTimer)) {
        timerRestart(genTimer);
        res = 1;
    }
 
    return res;
}

int defaultGenTimeFunc(float totalTimeElapsed,Timer* genTimer) {
    float timeInterval=50;
    if(totalTimeElapsed<1*timeInterval){
        timerSetInitialTime(genTimer,3);
     //   printf("gen3\n");
    }
    else if(totalTimeElapsed<2*timeInterval){
        timerSetInitialTime(genTimer,2);
      //  printf("gen2\n");
    }
    else if(totalTimeElapsed<3*timeInterval){
        timerSetInitialTime(genTimer,1);
     //   printf("gen1\n");
    }
    else if(totalTimeElapsed<4*timeInterval){
        timerSetInitialTime(genTimer,0.5);
    //    printf("gen0.5\n");
    }
     else if(totalTimeElapsed<5*timeInterval){
        timerSetInitialTime(genTimer,0.25);
     //   printf("gen0.25\n");
    }
    int res = 0;
    if (!timerIsRunning(genTimer)) {
        timerRestart(genTimer);
        res = 1;
    }
 
    return res;
}

struct monsterGen {
    Vector* monsterList;
    Vector* availableMonsterIdStack;
    Vector* deactivatedMonsterIdList;
    Vector* activeMonsterIdSorted; // indices of active monsters (see monsterGenGetNextActiveMonsterIndex)
    Path* path;
    
    Monster* (*monsterCreator)(Path*);

    int (*genTimeFunc)(float,Timer*);
    float totalTimeElapsed;
    Timer* genTimer;

    unsigned int numberOfInstantFinishedMonsters;
};

void freeMonsterList(void* m) {
    Monster* monster = *(Monster**)m;
    monsterFree(monster);
}

MonsterGen* monsterGenNew(Path* path, Monster* (*monsterCreator)(Path*), int (*genTimeFunc)(float,Timer*)) {
    MonsterGen* mg = malloc(sizeof(MonsterGen));
    mg->path = path;

    mg->monsterCreator = monsterCreator;

    mg->genTimeFunc = genTimeFunc;
    mg->totalTimeElapsed =0;
    mg->genTimer=timerNew(0);

    mg->monsterList = vectorCreate(sizeof(Monster*), freeMonsterList);
    mg->availableMonsterIdStack = vectorCreate(sizeof(int), NULL);
    mg->activeMonsterIdSorted = vectorCreate(sizeof(int), NULL);
    mg->deactivatedMonsterIdList = vectorCreate(sizeof(bool), NULL);
    mg->numberOfInstantFinishedMonsters = 0;

    

    return mg;
}

void monsterGenFree(MonsterGen* mg) {
    vectorFree(mg->monsterList);
    vectorFree(mg->availableMonsterIdStack);
    vectorFree(mg->deactivatedMonsterIdList);
    vectorFree(mg->activeMonsterIdSorted);
    timerFree(mg->genTimer);
    free(mg);
}

void monsterGenAddMonster(MonsterGen* mg) {
    Monster* new_monster = mg->monsterCreator(mg->path);
    vectorPushBack(mg->monsterList, (char*)&new_monster);
    vectorPushBackVal(mg->deactivatedMonsterIdList, false);

    vectorPushBackVal(mg->activeMonsterIdSorted, vectorGetSize(mg->monsterList) - 1);
}

void monsterGenUpdate(MonsterGen* mg, Arena* arena, Terminal* terminal, float delta_t) {
    mg->totalTimeElapsed+=delta_t;
    timerUpdate(mg->genTimer,delta_t);
    int n_monster_to_render = mg->genTimeFunc(mg->totalTimeElapsed,mg->genTimer);
     
     

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
        int monsterId = vectorGetVal(mg->activeMonsterIdSorted, i);
        if (!vectorGetVal(mg->deactivatedMonsterIdList, monsterId)) {
            vectorSetVal(mg->activeMonsterIdSorted, new_size, monsterId);
            new_size++;
        }
    }
    unsigned int n_removes = vectorGetSize(mg->activeMonsterIdSorted) - new_size;

    for (unsigned int i = 0; i < n_removes; i++) {
        vectorPopBack(mg->activeMonsterIdSorted);
    }
    int stackSize;
    int monsterId; 

    Vec2d startPos;
    pathGetPoint(mg->path,0,&startPos);
    
    while (n_monster_to_render > 0) {
        stackSize = vectorGetSize(mg->availableMonsterIdStack);
        if (stackSize > 0) {
            monsterId = vectorGetVal(mg->availableMonsterIdStack, stackSize - 1);
            Monster* m = *(Monster**)vectorGet(mg->monsterList, monsterId);
            monsterReset(m,&startPos );
            vectorPopBack(mg->availableMonsterIdStack);
            vectorSetVal(mg->deactivatedMonsterIdList, monsterId, false);
            vectorPushBackVal(mg->activeMonsterIdSorted, monsterId);
        } else {
            monsterGenAddMonster(mg);
            /*
            if (!(vectorGetSize(mg->monsterList) > 8)) {
               
            }
            */
        }

        n_monster_to_render--;
    }
}

int monsterGenGetNextActiveMonsterIndex(MonsterGen* mg, unsigned int sortedIndex) {
    if (vectorGetSize(mg->activeMonsterIdSorted) == 0 || sortedIndex >= vectorGetSize(mg->activeMonsterIdSorted)) {
        return -1;
    }
    return vectorGetVal(mg->activeMonsterIdSorted, sortedIndex);
}

Monster* monsterGenGetMonster(MonsterGen* mg, unsigned int index) {
    return *(Monster**)vectorGet(mg->monsterList, index);
}

unsigned int monsterGenGetNumberOfInstantFinishedMonsters(MonsterGen* mg) {

    return mg->numberOfInstantFinishedMonsters;
}

/*

 47
0.0, 480.0, 32.0, 480.0, 64.0, 480.0, 64.0, 448.0, 96.0, 448.0, 96.0, 416.0, 128.0, 416.0, 128.0, 384.0, 160.0, 384.0, 160.0, 352.0, 160.0, 320.0, 192.0, 320.0, 192.0, 288.0, 224.0, 288.0, 224.0, 256.0, 256.0, 256.0, 256.0, 224.0, 256.0, 192.0, 288.0, 192.0, 288.0, 160.0, 320.0, 160.0, 352.0, 160.0, 352.0, 128.0, 384.0, 128.0, 416.0, 128.0, 448.0, 128.0, 480.0, 128.0, 480.0, 160.0, 512.0, 160.0, 544.0, 160.0, 544.0, 192.0, 576.0, 192.0, 576.0, 224.0, 576.0, 256.0, 608.0, 256.0, 608.0, 288.0, 640.0, 288.0, 640.0, 320.0, 672.0, 320.0, 672.0, 352.0, 704.0, 352.0, 704.0, 384.0, 704.0, 416.0, 736.0, 416.0, 736.0, 448.0, 768.0, 448.0, 768.0, 480.0, 

47
0.0, 128.0, 32.0, 128.0, 64.0, 128.0, 64.0, 160.0, 96.0, 160.0, 96.0, 192.0, 128.0, 192.0, 128.0, 224.0, 160.0, 224.0, 160.0, 256.0, 160.0, 288.0, 192.0, 288.0, 192.0, 320.0, 224.0, 320.0, 224.0, 352.0, 256.0, 352.0, 256.0, 384.0, 256.0, 416.0, 288.0, 416.0, 288.0, 448.0, 320.0, 448.0, 352.0, 448.0, 352.0, 480.0, 384.0, 480.0, 416.0, 480.0, 448.0, 480.0, 480.0, 480.0, 480.0, 448.0, 512.0, 448.0, 544.0, 448.0, 544.0, 416.0, 576.0, 416.0, 576.0, 384.0, 576.0, 352.0, 608.0, 352.0, 608.0, 320.0, 640.0, 320.0, 640.0, 288.0, 672.0, 288.0, 672.0, 256.0, 704.0, 256.0, 704.0, 224.0, 704.0, 192.0, 736.0, 192.0, 736.0, 160.0, 768.0, 160.0, 768.0, 128.0, 
*/
