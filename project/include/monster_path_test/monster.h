
//#include "../../../lab5/spriteXMP.h"

//#include "../../resources/1.h"
#include "../../include/monster_path_test/vec2d.h"

#include <stdio.h>

//static SpriteXPM* sprite;

typedef struct a
{
  /* data */
}SpriteXPM;



typedef struct monster Monster;

Monster* monsterCreate(float x,float y,float vel_x,float vel_y, SpriteXPM* sprite);


Monster* monsterFree(Monster* m);

float monsterGetX(Monster* m);
float monsterGetY(Monster* m);

float monsterGetVelX(Monster* m);
float monsterGetVelY(Monster* m);

void monsterSetX(Monster* m,float x);
void monsterSetY(Monster* m,float y);
void monsterSetVelX(Monster* m,float vel_x);
void monsterSetVelY(Monster* m,float vel_y);

void monsterSetDir(Monster* m,float dx, float dy);
void monsterUpdatePos(Monster* m,double delta_t);
