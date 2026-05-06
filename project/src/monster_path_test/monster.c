#include "../../include/monster_path_test/monster.h"

struct monster
{
  float vel_x;
  float vel_y;
  float x;
  float y;
  SpriteXPM* sprite;
};




float monsterGetX(Monster* m){
  return m->x;
}
float monsterGetY(Monster* m){
  return m->y;
}

float monsterGetVelX(Monster* m){
  return m->vel_x;
}
float monsterGetVelY(Monster* m){
  return m->vel_y;
}

void monsterSetX(Monster* m,float x){
  m->x=x;
}
void monsterSetY(Monster* m,float y){
  m->y=y;
}

void monsterSetVelX(Monster* m,float dx){
  m->vel_x=dx;
}
void monsterSetVelY(Monster* m,float dy){
  m->vel_y=dy;
}
