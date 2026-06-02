#ifndef RIGIDBODY
#define RIGIDBODY

#include <stdbool.h>

#include "../arena.h"
#include "boxCollider.h"
#include "../utils/vec2d.h"

typedef struct RigidBody RigidBody;

RigidBody* rigidBodyNew();
void rigidBodyFree(RigidBody* rb);

void rigidBodyUpdate(RigidBody* rb, Vec2d* pos, Arena* arena, BoxCollider* hitBox, float deltaTime);
void rigidBodySetVelocity(RigidBody* rb, int xSpeed, int ySpeed);

#endif
