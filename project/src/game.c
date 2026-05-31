#include "../include/game.h"
#include "../include/arena.h"

#include "../resources/map/background.h"

static Arena* arena;

void startGame(Terminal* terminal) {
    arena = arenaNew(terminal, "/home/lcom/labs/project/resources/map/grid_walls.csv", "/home/lcom/labs/project/resources/map/grid_path.csv");
}

void exitGame() {
    arenaFree(arena);
}

void gameLoop(Terminal* terminal, float deltaTime) {
    arenaUpdate(arena, terminal, deltaTime);
}
