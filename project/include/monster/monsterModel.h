#include "../../include/utils/vec2d.h"

#include "../../include/monster/path.h"
#include "../../include/components/rigidBody.h"
#include "../../include/components/boxCollider.h"

#include <stdio.h>

typedef struct monsterModel MonsterModel;

unsigned int monsterModelSizeof();

MonsterModel* monsterModelCreate(float x,float y,float vel_x,float vel_y);


MonsterModel* monsterModelFree(MonsterModel* m);
 
RigidBody* monsterModelGetRigidBody(MonsterModel* m);
BoxCollider* monsterModelGetBoxCollider(MonsterModel* m);
Path* monsterModelGetPath(MonsterModel* m);

float monsterModelGetX(MonsterModel* m);
float monsterModelGetY(MonsterModel* m);

void monsterModelGetPos(MonsterModel*m,Vec2d* pos);
void monsterModelSetPos(MonsterModel* m,Vec2d* pos);


void monsterModelGetVel(MonsterModel* m,Vec2d* pos);
void monsterModelSetVel(MonsterModel* m,Vec2d* pos);

float monsterModelGetVelX(MonsterModel* m);
float monsterModelGetVelY(MonsterModel* m);

void monsterModelSetX(MonsterModel* m,float x);
void monsterModelSetY(MonsterModel* m,float y);
void monsterModelSetVelX(MonsterModel* m,float vel_x);
void monsterModelSetVelY(MonsterModel* m,float vel_y);

void monsterModelSetDir(MonsterModel* m,float dx, float dy);
void monsterModelSetDirWithVec(MonsterModel* m,Vec2d* v);



void monsterModelUpdatePos(MonsterModel* m,double delta_t);
