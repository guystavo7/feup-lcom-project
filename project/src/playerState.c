#include "../include/player/playerState.h"

void playerStateSetAct(PlayerState* state, void (*act)(PlayerState* state, PlayerModel* model, Terminal* terminal, Arena* arena, float deltaTime), PlayerState* (*transition)(PlayerState* state, PlayerModel* model)) {
	state->act = act;
	state->transition = transition;
}
