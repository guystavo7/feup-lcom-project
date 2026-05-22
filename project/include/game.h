#ifndef GAME
#define GAME

#include "../../lab5/terminal.h"

void startGame(Terminal* terminal);
void exitGame();
void gameLoop(Terminal* terminal, float deltaTime);

#endif
