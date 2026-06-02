#pragma once
#include "monsterModel.h"
#include "../components/animatedSprite.h"
typedef struct monsterView MonsterView;


MonsterView* monsterViewNew(MonsterModel* model,AnimatedSprite* animation);

void monsterViewFree(MonsterView* view);

void monsterViewUpdate (MonsterView* view, Terminal* terminal,float delta_t);
