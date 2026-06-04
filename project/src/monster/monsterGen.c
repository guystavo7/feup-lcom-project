#include "../../include/monster/monsterGen.h"

#include "../../include/arena.h"
#include "../../include/data.h"

#include <stdlib.h>

Monster* defaultMonsterCreator(Path* path){
    return alienMonsterNew(10,10,path);
}

static float counter=0;
int defaultGenTimeFunc(float delta){
    int res=0;
    if((int)counter%20==0 ){
        res= 1;
    }



    counter++;

    return res;
}

struct monsterGen{
    Vector* monsterList;
    Vector* availableMonsterIdStack;
    Vector* deactivatedMonsterIdList;
    Path* path;
    Monster* (*monsterCreator)(Path*);
    int (*genTimeFunc)(float);
    unsigned int numberOfInstantFinishedMonsters;
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
    mg->availableMonsterIdStack=vectorCreate(sizeof(int),NULL);

    mg->deactivatedMonsterIdList=vectorCreate(sizeof(bool),NULL);

    mg->numberOfInstantFinishedMonsters=0;

    return mg;
}

void monsterGenFree(MonsterGen* mg){
    vectorFree(mg->monsterList);
    vectorFree(mg->availableMonsterIdStack);
    vectorFree(mg->deactivatedMonsterIdList);
    free(mg);
}

void monsterGenAddMonster(MonsterGen* mg){ 
    Monster* new_monster= mg->monsterCreator(mg->path);
    vectorPushBack(mg->monsterList,(char*)&new_monster);
    vectorPushBackVal(mg->deactivatedMonsterIdList,false);
}

 


void monsterGenUpdate(MonsterGen* mg, Arena* arena,Terminal* terminal, float delta_t){
    int n_monster_to_render=mg->genTimeFunc(delta_t);

    mg->numberOfInstantFinishedMonsters=0;


    for(unsigned int i=0;i<vectorGetSize(mg->monsterList) ; i++){

        Monster* m=*(Monster**)vectorGet(mg->monsterList,i);
        monsterUpdate(m,arena,terminal,delta_t);

        
        bool m_in_stack=vectorGetVal(mg->deactivatedMonsterIdList,i);


        if( ( monsterIsDead(m) || monsterGetFinishedPath(m) ) &&!m_in_stack){
            vectorSetVal(mg->deactivatedMonsterIdList,i,true);
            vectorPushBack(mg->availableMonsterIdStack,(char*)&i);
            
            if( !(monsterIsDead(m))){
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

    int stackSize; 
    int monsterId;
    Vec2d v={10,10};

    while (n_monster_to_render>0)
    {   
        stackSize= vectorGetSize(mg->availableMonsterIdStack);
        if(stackSize>0){
            monsterId=vectorGetVal(mg->availableMonsterIdStack,stackSize-1);
            Monster* m=*(Monster**)vectorGet(mg->monsterList,monsterId);
            monsterReset(m,&v);
            vectorPopBack(mg->availableMonsterIdStack);
            vectorSetVal(mg->deactivatedMonsterIdList,monsterId,false);
        }
        else
        {
            if(!(vectorGetSize(mg->monsterList)>8)){
                monsterGenAddMonster(mg);
            }
        }
        

       n_monster_to_render--;
    }

}

int monsterGenGetNextActiveMonsterIndex(MonsterGen* mg, unsigned int begin){
    for(unsigned int i=begin;i!=vectorGetSize(mg->monsterList);i++){
        if(!vectorGetVal(mg->deactivatedMonsterIdList,i)){
            return i;
        }
    }

    return -1;
}

Monster* monsterGenGetMonster(MonsterGen* mg,unsigned int index){
    return *(Monster**)vectorGet(mg->monsterList,index);
}

unsigned int monsterGenGetNumberOfInstantFinishedMonsters(MonsterGen* mg){

    return mg->numberOfInstantFinishedMonsters;
}

