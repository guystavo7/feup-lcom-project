#include "monsterController.h"
#include "monsterModel.h"
#include "monsterView.h"

typedef struct monster Monster;


Monster* monsterNew(float x,float y,float sizeX,float sizeY,float velocity,int hp,AnimatedSprite* animation,Path* path);
void monsterSumHP(Monster* m,int val);

void monsterUpdate(Monster* m, Arena* arena,Terminal* terminal, float delta_t);
void monsterFree(Monster* m);


RigidBody* monsterGetRigidBody(Monster * m);

BoxCollider* monsterGetBoxCollider(Monster* m);
