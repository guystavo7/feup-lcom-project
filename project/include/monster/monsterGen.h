#include "monster.h"
#include "pawnMonster.h"
#include "alienMonster.h"

typedef struct monsterGen MonsterGen;
 

Monster* defaultMonsterCreator(Path* path);
void freeMonsterList(void* m);

MonsterGen* monsterGenNew(Path* path,Monster* (*monsterCreator)(Path*),int (*genTimeFunc)(float));

void monsterGenFree(MonsterGen* mg);
void monsterGenAddMonster(MonsterGen* mg);
 


void monsterGenUpdate(MonsterGen* mg, Arena* arena,Terminal* terminal, float delta_t);

Monster* defaultMonsterCreator(Path* path);

int defaultGenTimeFunc(float delta);
