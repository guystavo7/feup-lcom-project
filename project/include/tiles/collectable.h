#ifndef COLLECTABLE
#define COLLECTABLE

#include "../utils/vec2d.h"
#include "../../../lab5/spriteXMP.h"
#include <stdbool.h>

typedef struct Collectable Collectable;

Collectable* collectableNew(Vec2d position, Vec2d offset, Vec2d size, char** XPM);
void collectableFree(Collectable* c);

void collectableSetActive(Collectable* c, bool isActive);
bool collectableIsActive(Collectable* c);

void collectableSetX(Collectable* c, float x);
void collectableSetY(Collectable* c, float y);
void collectableSetPostion(Collectable* c, Vec2d position);

float collectableGetX(Collectable* c);
float collectableGetY(Collectable* c);

void collectableUpdate(Collectable* c, Terminal* terminal);

#endif
