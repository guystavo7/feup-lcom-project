#include "../include/game.h"
#include "../include/arena.h"

#include "../resources/cursor.h"
#include "../resources/popUp.h"
#include "../resources/button.h"

static Arena* arena;

void startGame() {
	arena = arenaNew("/home/lcom/labs/project/resources/map1_TileLayer1.csv");
}

void exitGame() {
    arenaFree(arena);
}

void gameLoop(Terminal* terminal, float deltaTime) {
    arenaUpdate(arena, terminal, deltaTime);
}
