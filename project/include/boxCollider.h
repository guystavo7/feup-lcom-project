#ifndef BOXCOLLIDER
#define BOXCOLLIDER

#include <stdbool.h>

typedef struct BoxCollider BoxCollider; 

BoxCollider* boxColliderNew(int x, int y, int width, int height, int tag);
void boxColliderFree(BoxCollider* boxCollider);

bool boxIsCollidingWithBox(BoxCollider* b1, BoxCollider* b2);
bool boxIsCollidingWithPoint(BoxCollider* b1, int x, int y);
void boxColliderUpdate(BoxCollider* b, int x, int y);

void boxColliderSetX(BoxCollider* b, int x);
void boxColliderSetY(BoxCollider* b, int y);

int boxColliderGetTag(BoxCollider* b);
int boxColliderGetWidth(BoxCollider* b);
int boxColliderGetHeight(BoxCollider* b);
int boxColliderGetX(BoxCollider* b);
int boxColliderGetY(BoxCollider* b);

#endif
