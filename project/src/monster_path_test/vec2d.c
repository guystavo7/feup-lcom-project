#include "../../include/monster_path_test/vec2d.h"

struct vec2d
{
  float x, y;
};


Vec2d* vec2dCreate(float x,float y){
  Vec2d* v=malloc(sizeof(Vec2d));
  v->x=x;
  v->y=y;
  return v;
}


void vec2dFree(Vec2d* v){
  free(v);
}

float vec2dGetX(Vec2d* v){
  return v->x;
}
float vec2dGetY(Vec2d* v){
  return v->y;
}

void vec2dSetX(Vec2d* v,float x){
  v->x=x;
}

void vec2dSetY(Vec2d* v,float y){
  v->y=y;
}

float vec2dModule(Vec2d* v){
  float m=sqrt(v->x*v->x+v->y*v->y);
  return m;
}

Vec2d* vec2dDir(Vec2d* v){
  if(v->x==0.0f && v->y==0.0f){
    return vec2dCreate(0,0);
  }
  float m=vec2dModule(v);
  Vec2d* d=vec2dCreate(v->x/m,v->y/m);
  return d;
}