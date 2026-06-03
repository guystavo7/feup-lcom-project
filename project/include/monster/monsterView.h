#pragma once
#include "monsterModel.h"
#include "../components/animatedSprite.h"
typedef struct monsterView MonsterView;


MonsterView* monsterViewNew(MonsterModel* model,AnimatedSprite* (*animationBuilder)());

void monsterViewFree(MonsterView* view);

void monsterViewUpdate (MonsterView* view, Terminal* terminal,float delta_t);
