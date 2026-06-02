#ifndef PLAYER_RUNNING
#define PLAYER_RUNNING

#include "playerState.h"

void runningAct(PlayerState* state, PlayerModel* model, Terminal* terminal, Arena* arena, float deltaTime);
PlayerState* runningTransition(PlayerState* state, PlayerModel* model);
void runningOnEnter(PlayerState* state, PlayerModel* model, Terminal* terminal, Arena* arena, float deltaTime);

PlayerState* getRunningState();

#endif
