#include "../game.h"
#include "../../../lab5/spriteXMP.h"
#include "../inputHandler.h"
#include "../../resources/1.h"

#include <stdio.h>

static SpriteXPM* sprite;
static int x = 50;
static int y = 50;

typedef struct monster Monster;

float monsterGetX(Monster* m);
float monsterGetY(Monster* m);

float monsterGetVelX(Monster* m);
float monsterGetVelY(Monster* m);

void monsterSetX(Monster* m,float x);
void monsterSetY(Monster* m,float y);
void monsterSetVelX(Monster* m,float dx);
void monsterSetVelY(Monster* m,float dy);
