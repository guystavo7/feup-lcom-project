#include "monster.h"
#include "pawnMonster.h"
#include "alienMonster.h"
#include "../utils/timer.h"
#include "spearManMonster.h"
#include "bossMonster.h"

typedef struct monsterGen MonsterGen;
 

Monster* defaultMonsterCreator(Path* path);
Monster* pawnMonsterCreator(Path* path);
Monster* bossMonsterCreator(Path* path);

Monster* alienMonsterCreator(Path* path);
Monster* spearManMonsterCreator(Path* path);

int progressiveTimeFunction50(float totalTimeElapsed,Timer* genTimer);


int progressiveTimeFunction100_1(float totalTimeElapsed,Timer* genTimer);

int progressiveTimeFunction100_2(float totalTimeElapsed,Timer* genTimer);

int progressiveTimeFunction200_1(float totalTimeElapsed,Timer* genTimer);
int progressiveTimeFunction200_2(float totalTimeElapsed,Timer* genTimer) ;
int singleAppear450(float totalTimeElapsed,Timer* genTimer) ;



int defaultGenTimeFunc(float delta,Timer*);
int constantSlowGenTimeFunc(float totalTimeElapsed,Timer* genTimer);

int constantSlowGenTimeFunc_1(float totalTimeElapsed,Timer* genTimer);
int constantSlowGenTimeFunc_2(float totalTimeElapsed,Timer* genTimer);

int singleSpawnGenTimeFunc(float totalTimeElapsed, Timer* genTimer);


void freeMonsterList(void* m);

MonsterGen* monsterGenNew(Path* path,Monster* (*monsterCreator)(Path*),int (*genTimeFunc)(float,Timer*));

void monsterGenFree(MonsterGen* mg);
void monsterGenAddMonster(MonsterGen* mg);
 


void monsterGenUpdate(MonsterGen* mg, Arena* arena,Terminal* terminal, float delta_t);


int monsterGenGetNextActiveMonsterIndex(MonsterGen* mg, unsigned int sortedIndex);
Monster* monsterGenGetMonster(MonsterGen* mg,unsigned int index);


unsigned int monsterGenGetNumberOfInstantFinishedMonsters(MonsterGen* mg);


 
