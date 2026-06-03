#include "../../include/monster/monster.h"

struct monster
{
    
    MonsterModel* model;
    MonsterView * view;
    MonsterController * controller;

};

unsigned int monsterSizeof(){
  return sizeof(Monster);
}


Monster* monsterNew(float x,float y,float sizeX,float sizeY,float velocity,int hp,AnimatedSprite* animation,Path* path){
  Monster* m=malloc(sizeof(Monster));
  m->model=monsterModelNew( x, y, sizeX, sizeY, velocity,0,hp);
  m->view=monsterViewNew(m->model,animation);
  m->controller= monsterControllerNew(m->model,path);
  return m;
}

void monsterSumHP(Monster* m,int val){
  monsterModelSumHP(m->model,val);
}


bool monsterIsDead(Monster* m){
  return monsterModelGetDead(m->model);
}

void monsterUpdate(Monster* m, Arena* arena,Terminal* terminal, float delta_t){
  monsterControllerUpdate(m->controller,arena,delta_t);
  monsterViewUpdate(m->view,terminal,delta_t);



}

void monsterFree(Monster* m){
  monsterModelFree(m->model);
  monsterViewFree(m->view);
  monsterControllerFree(m->controller);
  free(m);

}

RigidBody* monsterGetRigidBody(Monster * m){
  return monsterControllerGetRigidBody(m->controller);
}

BoxCollider* monsterGetBoxCollider(Monster* m){
  return monsterControllerGetBoxCollider(m->controller);
}

