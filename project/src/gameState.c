#include "../include/gameState.h"
#include "../include/arena.h"
#include "../include/button.h"
#include "../include/components/particle.h"
#include "../include/data.h"
#include "../include/utils/cursorHandler.h"
#include "../include/utils/inputHandler.h"

#include "../../lab5/spriteXMP.h"
#include "../resources/mainMenu/mainMenu.h"

#include <stdlib.h>

static bool pressedNewGame = false;
static bool pressedExit = false;

struct GameState {
    void (*act)(GameState* state, Terminal* terminal, float deltaTime);
    void (*free)(GameState* state);
    GameState* (*transition)(GameState* state, Terminal* terminal);
    Cursor* cursor;

    Arena* arena;

    SpriteXPM* logo;
    Button* newGameButton;
    Button* exitButton;
};

static GameState* gameStateNew(void (*act)(GameState* state, Terminal* terminal, float deltaTimem), void (*free)(GameState* state), GameState* (*transition)(GameState* state, Terminal* terminal)) {
    GameState* res = malloc(sizeof(GameState));
    res->act = act;
    res->free = free;
    res->transition = transition;

    return res;
}

void gameStateUpdate(GameState* state, Terminal* terminal, float deltaTime) {
    state->act(state, terminal, deltaTime);
    cursorUpdate(getCursor(), terminal, deltaTime);
}

void gameStateFree(GameState* state) {
    state->free(state);
}

GameState* gameStateTransition(GameState* state, Terminal* terminal) {
    return state->transition(state, terminal);
}

// GameLoop
static void runningGameAct(GameState* state, Terminal* terminal, float deltaTime) {
    arenaUpdate(state->arena, terminal, deltaTime);
}

static void runningGameFree(GameState* state) {
    arenaFree(state->arena);
    free(state);
}

static GameState* runningGameTransition(GameState* state, Terminal* terminal) {
    if (getKeyDown(getInputHandler(), KEYCODE_ESQ)) {
        return mainMenuNew(terminal);
    }

    return state;
}

GameState* runningGameNew(Terminal* terminal) {
    pressedNewGame = false;
    pressedExit = false;

    GameState* res = gameStateNew(runningGameAct, runningGameFree, runningGameTransition);
    res->arena = arenaNew(terminal, "/home/lcom/labs/project/resources/map/grid_walls.csv");

    return res;
}

static void newGameFn(Button* self) {
    pressedNewGame = true;
}

// Main Menu
static void mainMenuButtonsHandler(GameState* state) {
    Vec2d mousePos = {cursorGetX(getCursor()), cursorGetY(getCursor())};
    bool leftClick = cursorIsLeftButtonPressedDown(getCursor());

    if (boxIsCollidingWithPoint(buttonGetBoxCollider(state->newGameButton), mousePos)) {
        if (leftClick) buttonClick(state->newGameButton);
        buttonHover(state->newGameButton);

    } else {
        buttonReset(state->newGameButton);
    }

    if (boxIsCollidingWithPoint(buttonGetBoxCollider(state->exitButton), mousePos)) {
        if (leftClick) buttonClick(state->exitButton);
        buttonHover(state->exitButton);

    } else {
        buttonReset(state->exitButton);
    }
}

static void mainMenuAct(GameState* state, Terminal* terminal, float deltaTime) {
    drawSpriteXPM(state->logo, terminal, LOGO_POSITION.x, LOGO_POSITION.y, false);
    buttonDraw(state->newGameButton, terminal, deltaTime, false);
    buttonDraw(state->exitButton, terminal, deltaTime, false);
    mainMenuButtonsHandler(state);
}

static void mainMenuFree(GameState* state) {
    free(state->logo);
    buttonFree(state->newGameButton);
    buttonFree(state->exitButton);
    free(state);
}

static GameState* mainMenuTransition(GameState* state, Terminal* terminal) {
    GameState* newState = state;
    if (pressedNewGame) {
        newState = runningGameNew(terminal);
        pressedNewGame = false;
        pressedExit = false;

    } else if (pressedExit) {
        newState = NULL;
        pressedNewGame = false;
        pressedExit = false;
    }

    return newState;
}

static void exitGameFn(Button* self) {
    pressedExit = true;
}

GameState* mainMenuNew(Terminal* terminal) {
    pressedNewGame = false;
    pressedExit = false;

    GameState* res = gameStateNew(mainMenuAct, mainMenuFree, mainMenuTransition);
    SpriteXPM** newGameButtons = malloc(sizeof(SpriteXPM*) * NEW_GAME_BUTTON_FRAMES);
    SpriteXPM** exitButtons = malloc(sizeof(SpriteXPM*) * EXIT_BUTTON_FRAMES);
    res->logo = newSpriteXPM((xpm_map_t)mainMenuXPM[LOGO], XPM_8_8_8, 0, 0);

    // New Game Button
    SpriteXPM* buttonNormal = newSpriteXPM((xpm_map_t)mainMenuXPM[NEW_GAME_BUTTON_NORMAL], XPM_8_8_8, 0, 0);
    SpriteXPM* buttonPressed = newSpriteXPM((xpm_map_t)mainMenuXPM[NEW_GAME_BUTTON_PRESSED], XPM_8_8_8, 0, 0);
    newGameButtons[0] = buttonNormal;
    newGameButtons[1] = buttonPressed;

    res->newGameButton = buttonNew(NEW_GAME_BUTTON_POS.x, NEW_GAME_BUTTON_POS.y, NEW_GAME_BUTTON_SIZE.x, NEW_GAME_BUTTON_SIZE.y, particleNew(BUTTONS_FRAME_SPEED, newGameButtons, NEW_GAME_BUTTON_FRAMES, false), newGameFn, basicButtonHoverFn);

    // Exit Button
    buttonNormal = newSpriteXPM((xpm_map_t)mainMenuXPM[EXIT_BUTTON_NORMAL], XPM_8_8_8, 0, 0);
    buttonPressed = newSpriteXPM((xpm_map_t)mainMenuXPM[EXIT_BUTTON_PRESSED], XPM_8_8_8, 0, 0);
    exitButtons[0] = buttonNormal;
    exitButtons[1] = buttonPressed;
    res->exitButton = buttonNew(EXIT_BUTTON_POS.x, EXIT_BUTTON_POS.y, EXIT_BUTTON_SIZE.x, EXIT_BUTTON_SIZE.y, particleNew(BUTTONS_FRAME_SPEED, exitButtons, EXIT_BUTTON_FRAMES, false), exitGameFn, basicButtonHoverFn);

	clearEmptyBuffer(terminal);

    return res;
}
