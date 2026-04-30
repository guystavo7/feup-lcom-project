#ifndef CIRCLECOLLIDER
#define CIRCLECOLLIDER

typedef struct CircleCollider CircleCollider;

CircleCollider* newCircleCollider(int x, int y, int radius, int tag);
void freeCircleCollider(CircleCollider* c);
bool circleIsCollidingWithPoint(CircleCollider* c, int x, int y); 
void updateCircleCollider(CircleCollider* c, int x, int y);

int circleColliderGetTag(CircleCollider* c);

#endif
