#ifndef PLAYER_IDLE
#define PLAYER_IDLE

#include "playerState.h"

void idleAct(PlayerState* state, PlayerModel* model, Terminal* terminal, Arena* arena, float deltaTime);
PlayerState* idleTransition(PlayerState* state, PlayerModel* model);
void idleOnEnter(PlayerState* state, PlayerModel* model, Terminal* terminal, Arena* arena, float deltaTime);

PlayerState* getIdleState();

#endif

