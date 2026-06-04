#pragma once

#include "../../include/utils/vec2d.h"

#include "../../include/monster/path.h"
#include "../../include/components/rigidBody.h"
#include "../../include/components/boxCollider.h"



#define MonsterTag 1
#include <stdio.h>

typedef struct monsterModel MonsterModel;

 
MonsterModel* monsterModelNew(float x,float y,float sizeX,float sizeY,float velX,float velY,int hp);

 

void monsterModelFree(MonsterModel* m);



int monsterModelGetHP(MonsterModel* m);

void monsterModelSetHP(MonsterModel* m,int hp);

int monsterModelGetInitialHP(MonsterModel* m);

void monsterModelSetInitialHP(MonsterModel* m,int hp);

void monsterModelSumHP(MonsterModel* m,int val);

bool monsterModelGetDead(MonsterModel* m);

void monsterModelSetDead(MonsterModel* m,bool dead);

bool monsterModelGetFinishedPath(MonsterModel* m);

void monsterModelSetFinishedPath(MonsterModel* m,bool finishedPath);

 

float monsterModelGetX(MonsterModel* m);
float monsterModelGetY(MonsterModel* m);


float monsterModelGetSizeX(MonsterModel* m);
float monsterModelGetSizeY(MonsterModel* m);
Vec2d monsterModelGetSize(MonsterModel* m);

float monsterModelGetVelX(MonsterModel* m);
float monsterModelGetVelY(MonsterModel* m);



void monsterModelGetPos(MonsterModel*m,Vec2d* pos);


void monsterModelSetPos(MonsterModel* m,Vec2d* pos);

void monsterModelGetVel(MonsterModel* m,Vec2d* pos);

void monsterModelSetVel(MonsterModel* m,Vec2d* pos);





void monsterModelSetX(MonsterModel* m,float x);
void monsterModelSetY(MonsterModel* m,float y);



void monsterModelSetVelX(MonsterModel* m,float x);
void monsterModelSetVelY(MonsterModel* m,float y);



void monsterModelSetDir(MonsterModel* m,float dx, float dy);
void monsterModelSetDirWithVec(MonsterModel* m,Vec2d* v);

