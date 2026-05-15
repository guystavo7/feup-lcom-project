#ifndef RIGIDBODY
#define RIGIDBODY

#include <stdbool.h>

#include "arena.h"
#include "boxCollider.h"

typedef struct RigidBody RigidBody;

RigidBody* rigidBodyNew(int x, int y, int width, int height, int tag);
void rigidBodyFree(RigidBody* rb);

void rigidBodyUpdate(RigidBody* rb, Arena* arena, float deltaTime);
void rigidBodySetVelocity(RigidBody* rb, int xSpeed, int ySpeed);

BoxCollider* rigidBodyGetBoxCollider(RigidBody* rb);

int rigidBodyGetX(RigidBody* rb);
int rigidBodyGetY(RigidBody* rb);

#endif
