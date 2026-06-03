#include "../../include/monster/monsterView.h" 
struct monsterView{
    MonsterModel* model;
    AnimatedSprite* animation;

    bool flipped;

};

MonsterView* monsterViewNew(MonsterModel* model,AnimatedSprite* animation){
  MonsterView* view =malloc(sizeof(MonsterView));
  view->model=model; 
  view->animation= animation; 
  return view;
}

void monsterViewFree(MonsterView* view){ 
  free(view);
}


void monsterViewUpdate (MonsterView* view, Terminal* terminal,float delta_t){ 
  if(monsterModelGetVelX(view->model)>0){
    view->flipped=false;
  }
  else if (!(monsterModelGetVelX(view->model)==0))
  { 
    view->flipped=true;
  }
  if(!monsterModelGetDead(view->model) && !monsterModelGetFinishedPath(view->model)){
   animatedSpriteUpdate(monsterModelGetX(view->model),monsterModelGetY(view->model),view->animation,terminal,delta_t,view->flipped);
  }
}
