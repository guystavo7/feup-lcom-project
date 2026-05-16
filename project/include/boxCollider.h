#ifndef BOXCOLLIDER
#define BOXCOLLIDER

#include "vec2d.h"
#include <stdbool.h>

typedef struct BoxCollider BoxCollider; 

BoxCollider* boxColliderNew(Vec2d pos, Vec2d offset, Vec2d size, int tag);
void boxColliderFree(BoxCollider* boxCollider);

bool boxIsCollidingWithBox(BoxCollider* b1, BoxCollider* b2);
bool boxIsCollidingWithPoint(BoxCollider* b1, Vec2d pos);

int boxColliderGetTag(BoxCollider* b);
int boxColliderGetWidth(BoxCollider* b);
int boxColliderGetHeight(BoxCollider* b);

void boxColliderSetX(BoxCollider* b, int x);
void boxColliderSetY(BoxCollider* b, int y);
void boxColliderSetPos(BoxCollider* b, Vec2d pos);

int boxColliderGetOffsetX(BoxCollider* b);
int boxColliderGetOffsetY(BoxCollider* b);
int boxColliderGetX(BoxCollider* b);
int boxColliderGetY(BoxCollider* b);

#endif
