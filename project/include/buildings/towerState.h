#ifndef TOWER_STATE 
#define TOWER_STATE 

typedef struct TowerState TowerState;

TowerState* playerStateNew();
void playerStateFree(TowerState* state);

void playerStateSetFunctions(PlayerState* state, void (*act)(PlayerState* state, PlayerModel* model, Terminal* terminal, Arena* arena, float deltaTime), void (*onEnter)(PlayerState* state, PlayerModel* model, Terminal* terminal, Arena* arena, float deltaTime), PlayerState* (*transition)(PlayerState* state, PlayerModel* model));

void playerStateOnEnter(PlayerState* state, PlayerModel* model, Terminal* terminal, Arena* arena, float deltaTime);
void playerStateAct(PlayerState* state, PlayerModel* model, Terminal* terminal, Arena* arena, float deltaTime);
PlayerState* playerStateTransition(PlayerState* state, PlayerModel* model);

#endif
