/*#include "../../include/monster/monster.h"

struct monster
{
  Vec2d* vel;
  Vec2d* pos;
  
};

unsigned int monsterSizeof(){
  return sizeof(Monster);
}

Monster* monsterCreate(float x,float y,float vel_x,float vel_y){
  Monster* m=malloc(sizeof(Monster));
  m->pos=vec2dCreate(x,y);
  m->vel=vec2dCreate(vel_x,vel_y);
  
  return m;
}


Monster* monsterFree(Monster* m){
  vec2dFree(m->pos);
  vec2dFree(m->vel);
  free(m);
  return NULL;
}

float monsterGetX(Monster* m){
  return vec2dGetX(m->pos);
}
float monsterGetY(Monster* m){
  return vec2dGetY(m->pos);
}

float monsterGetVelX(Monster* m){
  return vec2dGetX(m->vel);
}
float monsterGetVelY(Monster* m){
  return vec2dGetY(m->vel);
}


void monsterGetPos(Monster*m,Vec2d* pos){
  vec2dSetX(pos,vec2dGetX(m->pos));
  vec2dSetY(pos,vec2dGetY(m->pos));
}

void monsterSetX(Monster* m,float x){
  vec2dSetX(m->pos,x);
}
void monsterSetY(Monster* m,float y){
  vec2dSetY(m->pos,y);
}

void monsterSetVelX(Monster* m,float x){
  vec2dSetX(m->vel,x);
}
void monsterSetVelY(Monster* m,float y){
  vec2dSetY(m->vel,y);
}

void monsterSetDir(Monster* m,float dx, float dy){
  Vec2d* v=vec2dCreate(dx,dy);
  monsterSetDirWithVec(m,v);
  free(v);
}

void monsterSetDirWithVec(Monster* m,Vec2d* v){
  float dm2=vec2dModule2(v);
  float cur_m=vec2dModule(m->vel);
  if( (dm2>(1-EPSILON)) && ((1-EPSILON)>dm2) ){
    
    vec2dSetWithVec2d(m->vel,v);
    vec2dMultiply(m->vel,cur_m);
  }
  else{
    
    vec2dSetWithVec2d(m->vel,v);
    vec2dNormalize(m->vel);
    vec2dMultiply(m->vel,cur_m);
  }
}

void monsterUpdatePos(Monster* m,double delta_t){
  struct Vec2d a={m->vel->x, m->vel->y};
  vec2dMultiply(&a,delta_t);
  vec2dAddVec(m->pos,&a);
  vec2dFree(&a);
}
*/