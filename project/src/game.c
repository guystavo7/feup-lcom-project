#include "../include/game.h"
#include "../include/arena.h"

#include "../resources/map/background.h"

static Arena* arena;

#define ARENA_MAP_DIR "/home/lcom/labs/grupo_2leic13_2/project/resources/map/"

void startGame(Terminal* terminal) {
    arena = arenaNew(terminal, ARENA_MAP_DIR "grid_walls.csv", ARENA_MAP_DIR "grid_path.csv", ARENA_MAP_DIR "grid_water.csv");
}

void exitGame() {
    arenaFree(arena);
}

void gameLoop(Terminal* terminal, float deltaTime) {
    arenaUpdate(arena, terminal, deltaTime);
}
