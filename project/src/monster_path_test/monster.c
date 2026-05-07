#include "../../include/monster_path_test/monster.h"

struct monster
{
  Vec2d* vel;
  Vec2d* pos;
  SpriteXPM* sprite;
};


Monster* monsterCreate(float x,float y,float vel_x,float vel_y, SpriteXPM* sprite){
  Monster* m=malloc(sizeof(Monster));
  m->pos=vec2dCreate(x,y);
  m->vel=vec2dCreate(vel_x,vel_y);
  m->sprite=sprite;
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
  float dm2=dx*dx+dy*dy;
  float cur_m=vec2dModule(m->vel);
  if( (dm2>(1-EPSILON)) && ((1-EPSILON)>dm2) ){
    vec2dSetX(m->vel,dx*cur_m);
    vec2dSetY(m->vel,dy*cur_m);
  }
  else{
    vec2dFree(m->vel);
    m->vel=vec2dCreate(dx,dy);
    vec2dNormalize(m->vel);
    vec2dMultiply(m->vel,cur_m);
  }
}

void monsterUpdatePos(Monster* m,double delta_t){
  Vec2d* a=vec2dCreate(vec2dGetX(m->vel),vec2dGetY(m->vel));
  vec2dMultiply(a,delta_t);
  vec2dAddVec(m->pos,a);
  vec2dFree(a);
}