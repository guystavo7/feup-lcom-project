#ifndef PLAYER_STATE 
#define PLAYER_STATE 

#include "playerModel.h"

typedef struct PlayerState PlayerState;

PlayerState* playerStateNew();
void playerStateFree(PlayerState* state);

void playerStateSetFunctions(PlayerState* state, void (*act)(PlayerState* state, PlayerModel* model, Terminal* terminal, Arena* arena, float deltaTime), void (*onEnter)(PlayerState* state, PlayerModel* model, Terminal* terminal, Arena* arena, float deltaTime), PlayerState* (*transition)(PlayerState* state, PlayerModel* model));

void playerStateOnEnter(PlayerState* state, PlayerModel* model, Terminal* terminal, Arena* arena, float deltaTime);
void playerStateAct(PlayerState* state, PlayerModel* model, Terminal* terminal, Arena* arena, float deltaTime);
PlayerState* playerStateTransition(PlayerState* state, PlayerModel* model);

#endif
