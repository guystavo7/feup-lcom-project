#ifndef BOXCOLLIDER
#define BOXCOLLIDER

typedef struct BoxCollider BoxCollider; 

BoxCollider* newBoxCollider(int x, int y, int width, int height, int tag);
void freeBoxCollider(BoxCollider* boxCollider);

bool boxIsCollidingWithBox(BoxCollider* b1, BoxCollider* b2);
bool boxIsCollidingWithPoint(BoxCollider* b1, int x, int y);
void updateBoxCollider(BoxCollider* b, int x, int y);

int boxColliderGetTag(BoxCollider* b);

#endif
