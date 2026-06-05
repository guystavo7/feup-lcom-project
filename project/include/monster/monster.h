#pragma once
#include "monsterController.h"
#include "monsterModel.h"
#include "monsterView.h"
#include "../data.h"

typedef struct monster Monster;


unsigned int monsterSizeof();

Monster* monsterNew(float x,float y,float sizeX,float sizeY,float velocity,int hp,AnimatedSprite* (*animationBuilder)(),Path* path);

void monsterSumHP(Monster* m,int val);

bool monsterIsDead(Monster* m);


bool monsterGetFinishedPath(Monster* m);

void monsterUpdate(Monster* m, Arena* arena,Terminal* terminal, float delta_t);
void monsterFree(Monster* m);


RigidBody* monsterGetRigidBody(Monster * m);

BoxCollider* monsterGetBoxCollider(Monster* m);


void monsterReset(Monster* m, Vec2d* pos);

Vec2d monsterGetFutureDisplacement(Monster* m,float delta_t);

