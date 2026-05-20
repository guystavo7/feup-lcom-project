#ifndef BOXCOLLIDER
#define BOXCOLLIDER

#include "../utils/vec2d.h"
#include <stdbool.h>

typedef struct BoxCollider BoxCollider; 

BoxCollider* boxColliderNew(Vec2d pos, Vec2d offset, Vec2d size, int tag);
void boxColliderFree(BoxCollider* boxCollider);

bool boxIsCollidingWithBox(BoxCollider* b1, BoxCollider* b2);
bool boxIsCollidingWithPoint(BoxCollider* b1, Vec2d pos);

int boxColliderGetTag(BoxCollider* b);
float boxColliderGetWidth(BoxCollider* b);
float boxColliderGetHeight(BoxCollider* b);

void boxColliderSetX(BoxCollider* b, int x);
void boxColliderSetY(BoxCollider* b, int y);
void boxColliderSetPos(BoxCollider* b, Vec2d pos);

float boxColliderGetOffsetX(BoxCollider* b);
float boxColliderGetOffsetY(BoxCollider* b);
float boxColliderGetX(BoxCollider* b);
float boxColliderGetY(BoxCollider* b);

#endif
