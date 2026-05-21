#include "../../include/vec2d.h"
#include <stdio.h>

typedef struct monster Monster;

unsigned int monsterSizeof();

Monster* monsterCreate(float x,float y,float vel_x,float vel_y);


Monster* monsterFree(Monster* m);

float monsterGetX(Monster* m);
float monsterGetY(Monster* m);

void monsterGetPos(Monster*m,Vec2d* pos);

float monsterGetVelX(Monster* m);
float monsterGetVelY(Monster* m);

void monsterSetX(Monster* m,float x);
void monsterSetY(Monster* m,float y);
void monsterSetVelX(Monster* m,float vel_x);
void monsterSetVelY(Monster* m,float vel_y);

void monsterSetDir(Monster* m,float dx, float dy);
void monsterSetDirWithVec(Monster* m,Vec2d* v);



void monsterUpdatePos(Monster* m,double delta_t);
