#ifndef GAME
#define GAME

#include "../../lab5/terminal.h"

void startGame(Terminal* terminal);
void exitGame();
bool gameLoop(Terminal* terminal, float deltaTime);

#endif
