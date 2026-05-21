#ifndef GAME
#define GAME

#include "../../lab5/terminal.h"
#include "button.h"

void startGame();
void exitGame();
void gameLoop(Terminal* terminal, float deltaTime);
UIManager* getUIManager(void);

#endif
