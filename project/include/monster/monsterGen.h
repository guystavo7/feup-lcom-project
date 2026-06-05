#include "monster.h"
#include "pawnMonster.h"
#include "alienMonster.h"
#include "../utils/timer.h"

typedef struct monsterGen MonsterGen;
 

Monster* defaultMonsterCreator(Path* path);
Monster* pawnMonsterCreator(Path* path);



int defaultGenTimeFunc(float delta,Timer*);
int constantSlowGenTimeFunc(float totalTimeElapsed,Timer* genTimer) ;


void freeMonsterList(void* m);

MonsterGen* monsterGenNew(Path* path,Monster* (*monsterCreator)(Path*),int (*genTimeFunc)(float,Timer*));

void monsterGenFree(MonsterGen* mg);
void monsterGenAddMonster(MonsterGen* mg);
 


void monsterGenUpdate(MonsterGen* mg, Arena* arena,Terminal* terminal, float delta_t);


int monsterGenGetNextActiveMonsterIndex(MonsterGen* mg, unsigned int sortedIndex);
Monster* monsterGenGetMonster(MonsterGen* mg,unsigned int index);


unsigned int monsterGenGetNumberOfInstantFinishedMonsters(MonsterGen* mg);


 
