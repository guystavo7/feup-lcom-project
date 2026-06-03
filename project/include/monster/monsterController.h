#pragma once
#include "monsterModel.h"

typedef struct monsterController MonsterController;
 

MonsterController* monsterControllerNew(MonsterModel* model,Path * path);


void monsterControllerUpdate(MonsterController* controller,Arena*arena,float delta_t);
void monsterControllerFree(MonsterController* controller);

RigidBody* monsterControllerGetRigidBody(MonsterController* m);

BoxCollider* monsterControllerGetBoxCollider(MonsterController* m);
Path* monsterControllerGetPath(MonsterController* m);
void monsterControllerGoBackToStart(MonsterController* m);
