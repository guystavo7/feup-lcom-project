#ifndef CIRCLECOLLIDER
#define CIRCLECOLLIDER

typedef struct CircleCollider CircleCollider;

#include <stdbool.h>

CircleCollider* circleColliderNew(int x, int y, int radius, int tag);
void circleColliderFree(CircleCollider* c);
bool circleIsCollidingWithPoint(CircleCollider* c, int x, int y); 
void circleColliderUpdate(CircleCollider* c, int x, int y);

int circleColliderGetTag(CircleCollider* c);

#endif
