#ifndef PLAYER_STATE 
#define PLAYER_STATE 

#include "playerModel.h"

typedef struct PlayerState PlayerState;

struct PlayerState {
    void (*act)(PlayerState* state, PlayerModel* model, Terminal* terminal, Arena* arena, float deltaTime);
    void (*onEnter)(PlayerState* state, PlayerModel* model, Terminal* terminal, Arena* arena, float deltaTime);
    PlayerState* (*transition)(PlayerState* state, PlayerModel* model);
};

void playerStateSetAct(PlayerState* state, void (*act)(PlayerState* state, PlayerModel* model, Terminal* terminal, Arena* arena, float deltaTime), PlayerState* (*transition)(PlayerState* state, PlayerModel* model));

#endif
