#ifndef PLAYER_STATE
#define PLAYER_STATE

#include "../../lab5/terminal.h"

typedef struct GameState GameState; 

void gameStateUpdate(GameState* state, Terminal* terminal, float deltaTime);
void gameStateFree(GameState* state);
GameState* gameStateTransition(GameState* state, Terminal* terminal);

GameState* runningGameNew(Terminal* terminal);
GameState* mainMenuNew();

#endif
