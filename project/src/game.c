#include "../include/game.h"
#include "../include/arena.h"
#include "../include/gameState.h"

#include "../resources/map/background.h"

static GameState* state;

void startGame(Terminal* terminal) {
    state = mainMenuNew();
}

void exitGame() {
    gameStateFree(state);
}

bool gameLoop(Terminal* terminal, float deltaTime) {
    gameStateUpdate(state, terminal, deltaTime);
    GameState* newState = gameStateTransition(state, terminal);
	if (!newState) return false;

    if (newState != state) {
        gameStateFree(state);
        state = newState;
    }

    return true;
}
