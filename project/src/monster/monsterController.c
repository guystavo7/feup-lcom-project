#include "../../include/monster/monsterController.h"

struct monsterController
{
    MonsterModel* model;


};

MonsterController* monsterControllerNew(MonsterModel* model){
  MonsterModel* m=malloc(sizeof(MonsterController));
  
  return m;
}

void monsterControllerUpdate(MonsterController* controller,float delta_t){
    MonsterModel *m=controller->model;
    Vec2d delta_x,monsterPos;
    
    monsterModelGetVel(m,&delta_x);
    vec2dMultiply(&delta_x,delta_t);
    
    vec2dAddVec(,&a);
    vec2dFree(&a);
}

MonsterController* monsterControllerFree(MonsterController* controller){
    monsterModelFree(controller->model);
    free(controller);
}