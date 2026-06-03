#include "../../include/monster/monsterGen.h"

Monster* defaultMonsterCreator(Path* path){
    return alienMonsterNew(10,10,path);
}

static float counter=0;
int defaultGenTimeFunc(float delta){
    int res=0;
    if((int)counter%10==0 && !(counter>80)){
        res= 1;
    }



    counter++;

    return res;
}

struct monsterGen{
    Vector* monsterList;
    Vector* availableMonsterStack;
    Vector* monsterInStack;
    Path* path;
    Monster* (*monsterCreator)(Path*);
    int (*genTimeFunc)(float);
};

void freeMonsterList(void* m){
    Monster* monster=*(Monster**)m;
    monsterFree(monster);
}

MonsterGen* monsterGenNew(Path* path,Monster* (*monsterCreator)(Path*),int (*genTimeFunc)(float)){
    MonsterGen* mg=malloc(sizeof(MonsterGen));
    mg->path=path;
    mg->monsterCreator=monsterCreator;
    mg->genTimeFunc=genTimeFunc;

    mg->monsterList=vectorCreate(sizeof(Monster*),freeMonsterList);
    mg->availableMonsterStack=vectorCreate(sizeof(Monster*),NULL);

    mg->monsterInStack=vectorCreate(sizeof(bool),NULL);

    return mg;
}

void monsterGenFree(MonsterGen* mg){
    vectorFree(mg->monsterList);
    vectorFree(mg->availableMonsterStack);
    vectorFree(mg->monsterInStack);
    free(mg);
}

void monsterGenAddMonster(MonsterGen* mg){ 
    Monster* new_monster= mg->monsterCreator(mg->path);
    vectorPushBack(mg->monsterList,(char*)&new_monster);
}

 


void monsterGenUpdate(MonsterGen* mg, Arena* arena,Terminal* terminal, float delta_t){
    int n_monster_to_render=mg->genTimeFunc(delta_t);


    for(unsigned int i=0;i<vectorGetSize(mg->monsterList) ; i++){

        Monster* m=*(Monster**)vectorGet(mg->monsterList,i);
        monsterUpdate(m,arena,terminal,delta_t);

        /*
        bool m_in_stack=vectorGet(mg->monsterInStack,i);

        if(monsterIsDead(m) &&!m_in_stack){
            vectorSetVal(mg->monsterInStack,i,true);

            vectorPushBack(mg->availableMonsterStack,&m);

        }*/

    }

    while (n_monster_to_render>0)
    {
        /*
        if(vectorGetSize(mg->availableMonsterStack)>0){

        }*/
       monsterGenAddMonster(mg);

       n_monster_to_render--;
    }

    



}
