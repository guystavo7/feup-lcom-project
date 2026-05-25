#include "../../include/monster/monsterModel.h"


struct monsterModel
{
  Vec2d vel;
  Vec2d pos;
  char* walkingAnimationFolderPath;

  RigidBody *rb;
  BoxCollider * bc;
  Path * path;


};

unsigned int monsterSizeof(){
  return sizeof(MonsterModel);
}

MonsterModel* monsterModelNew(float x,float y,float vel_x,float vel_y,char* walkingAnimationFolderPath,RigidBody *rb, BoxCollider * bc,Path * path){
  MonsterModel* m=malloc(sizeof(MonsterModel));
  m->pos= (Vec2d){x,y};
  m->vel=(Vec2d){vel_x,vel_y};
  m->walkingAnimationFolderPath=walkingAnimationFolderPath;
  m->rb=rb;
  m->bc=bc;
  
  return m;
}


MonsterModel* monsterModelFree(MonsterModel* m){ 
  rigidBodyFree(m->rb);
  boxColliderFree(m->bc);
  free(m);
  return NULL;
}


 

RigidBody* monsterModelGetRigidBody(MonsterModel* m){
  return m->rb;
}

BoxCollider* monsterModelGetBoxCollider(MonsterModel* m){
  return m->bc;
}

Path* monsterModelGetPath(MonsterModel* m){
  return m->path;
}


float monsterModelGetX(MonsterModel* m){
  return vec2dGetX(&m->pos);
}
float monsterModelGetY(MonsterModel* m){
  return vec2dGetY(&m->pos);
}

float monsterModelGetVelX(MonsterModel* m){
  return vec2dGetX(&m->vel);
}
float monsterModelGetVelY(MonsterModel* m){
  return vec2dGetY(&m->vel);
}



void monsterModelGetPos(MonsterModel*m,Vec2d* pos){
  vec2dSetX(pos,vec2dGetX(&m->pos));
  vec2dSetY(pos,vec2dGetY(&m->pos));
}


void monsterModelSetPos(MonsterModel* m,Vec2d* pos){
  vec2dSetWithVec2d(&m->pos,pos);
}


void monsterModelGetVel(MonsterModel* m,Vec2d* pos){
    vec2dSetWithVec2d(pos,&m->vel);
}

void monsterModelSetVel(MonsterModel* m,Vec2d* pos){
    vec2dSetWithVec2d(&m->vel,pos);
}





void monsterModelSetX(MonsterModel* m,float x){
  vec2dSetX(&m->pos,x);
}
void monsterModelSetY(MonsterModel* m,float y){
  vec2dSetY(&m->pos,y);
}

void monsterModelSetVelX(MonsterModel* m,float x){
  vec2dSetX(&m->vel,x);
}
void monsterModelSetVelY(MonsterModel* m,float y){
  vec2dSetY(&m->vel,y);
}



void monsterModelSetDir(MonsterModel* m,float dx, float dy){
  Vec2d* v=vec2dCreate(dx,dy);
  monsterModelSetDirWithVec(m,v);
  free(v);
}

void monsterModelSetDirWithVec(MonsterModel* m,Vec2d* v){
  float dm2=vec2dModule2(v);
  float cur_m=vec2dModule(&m->vel);
  if( (dm2>(1-EPSILON)) && ((1-EPSILON)>dm2) ){
    
    vec2dSetWithVec2d(&m->vel,v);
    vec2dMultiply(&m->vel,cur_m);
  }
  else{
    
    vec2dSetWithVec2d(&m->vel,v);
    vec2dNormalize(&m->vel);
    vec2dMultiply(&m->vel,cur_m);
  }
}

/*
void monsterModelUpdatePos(MonsterModel* m,double delta_t){
  struct Vec2d a={m->vel.x, m->vel.y};
  vec2dMultiply(&a,delta_t);
  vec2dAddVec(&m->pos,&a);
  vec2dFree(&a);
}
  */
