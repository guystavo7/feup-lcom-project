#include "../include/gameState.h"
#include "../include/arena.h"
#include "../include/button.h"
#include "../include/data.h"
#include "../include/utils/cursorHandler.h"
#include "../include/utils/inputHandler.h"

#include "../../lab5/spriteXMP.h"
#include "../resources/mainMenu/mainMenu.h"
#include "../resources/pauseMenu/pauseMenu.h"

#include <stdlib.h>

static bool pressedNewGame = false;
static bool pressedExit = false;
static bool pressedGoBack = false;
static bool pressedPause = false;

struct GameState {
    void (*act)(GameState* state, Terminal* terminal, float deltaTime);
    void (*free)(GameState* state);
    GameState* (*transition)(GameState* state, Terminal* terminal);
    Cursor* cursor;

    // GameLoop
    Arena* arena;

    // Main menu
    SpriteXPM* logo;
    Button* newGameButton;
    Button* closeGameButton;

    // Pause Menu
    SpriteXPM* pauseMenu;
    Button* resumeButton;
    Button* helpButton;
    Button* goBackMainMenuButton;
};

static void resetBools() {
    pressedExit = false;
    pressedGoBack = false;
    pressedNewGame = false;
	pressedPause = false;
}

static GameState* gameStateNew(void (*act)(GameState* state, Terminal* terminal, float deltaTimem), void (*free)(GameState* state), GameState* (*transition)(GameState* state, Terminal* terminal)) {
	resetBools();
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
static void runningGameFree(GameState* state) {
    buttonFree(state->resumeButton);
    buttonFree(state->goBackMainMenuButton);
    buttonFree(state->helpButton);
    freeSpriteXPM(state->pauseMenu);
    arenaFree(state->arena);

    free(state);
}

static GameState* runningGameTransition(GameState* state, Terminal* terminal) {
    GameState* newState = state;
    if (getKeyDown(getInputHandler(), KEYCODE_ESQ)) {
        pressedPause = true;
    }

    if (buttonIsFinished(state->goBackMainMenuButton) && pressedGoBack) {
        resetBools();
        newState = mainMenuNew(terminal);
    }
    return newState;
}

static void resumeFn(Button* self) {
	pressedPause = false;
}

static void helpFn(Button* self) {
    printf("Help\n");
}

static void goBackMainMenuFn(Button* self) {
    pressedGoBack = true;
}

static void pausedMenuButtonsHandler(GameState* state) {
    Vec2d mousePos = {cursorGetX(getCursor()), cursorGetY(getCursor())};
    bool leftClick = cursorIsLeftButtonPressedDown(getCursor());

    if (boxIsCollidingWithPoint(buttonGetBoxCollider(state->resumeButton), mousePos)) {
        if (leftClick) buttonClick(state->resumeButton);
        buttonHover(state->resumeButton);

    } else {
        buttonReset(state->resumeButton);
    }

    if (boxIsCollidingWithPoint(buttonGetBoxCollider(state->helpButton), mousePos)) {
        if (leftClick) buttonClick(state->helpButton);
        buttonHover(state->helpButton);

    } else {
        buttonReset(state->helpButton);
    }

    if (boxIsCollidingWithPoint(buttonGetBoxCollider(state->goBackMainMenuButton), mousePos)) {
        if (leftClick) buttonClick(state->goBackMainMenuButton);
        buttonHover(state->goBackMainMenuButton);

    } else {
        buttonReset(state->goBackMainMenuButton);
    }
}

static void runningGameAct(GameState* state, Terminal* terminal, float deltaTime) {
    if (pressedPause) {
        drawSpriteXPM(state->pauseMenu, terminal, PAUSE_MENU_BG_POS.x, PAUSE_MENU_BG_POS.y, false);
        buttonDraw(state->resumeButton, terminal, deltaTime, false);
        buttonDraw(state->helpButton, terminal, deltaTime, false);
        buttonDraw(state->goBackMainMenuButton, terminal, deltaTime, false);
        pausedMenuButtonsHandler(state);
    } else {
        arenaUpdate(state->arena, terminal, deltaTime);
    }
}

GameState* runningGameNew(Terminal* terminal) {
    GameState* res = gameStateNew(runningGameAct, runningGameFree, runningGameTransition);
    res->arena = arenaNew(terminal, map1, path1, lake1);
    res->pauseMenu = newSpriteXPM((xpm_map_t)pauseMenuXPM[PAUSE_MENU_INDEX], XPM_8_8_8, 0, 0);

    // Resume Button
    SpriteXPM** resumeSprites = malloc(sizeof(SpriteXPM*));
    SpriteXPM* buttonNormal = newSpriteXPM((xpm_map_t)pauseMenuXPM[RESUME_BUTTON_INDEX], XPM_8_8_8, 0, 0);
    resumeSprites[0] = buttonNormal;
    res->resumeButton = buttonNew(RESUME_POS.x, RESUME_POS.y, PAUSE_MENU_ICON_SIZE.x, PAUSE_MENU_ICON_SIZE.y, animatedSpriteNew(BUTTONS_FRAME_SPEED, resumeSprites, 1, NORMAL), resumeFn, basicButtonHoverFn);

    // Help button
    SpriteXPM** helpSprites = malloc(sizeof(SpriteXPM*));
    buttonNormal = newSpriteXPM((xpm_map_t)pauseMenuXPM[HELP_BUTTON_INDEX], XPM_8_8_8, 0, 0);
    helpSprites[0] = buttonNormal;
    res->helpButton = buttonNew(HELP_POS.x, HELP_POS.y, PAUSE_MENU_ICON_SIZE.x, PAUSE_MENU_ICON_SIZE.y, animatedSpriteNew(BUTTONS_FRAME_SPEED, helpSprites, 1, NORMAL), helpFn, basicButtonHoverFn);

    // Go back button
    SpriteXPM** goBackMainMenuSprites = malloc(sizeof(SpriteXPM*));
    buttonNormal = newSpriteXPM((xpm_map_t)pauseMenuXPM[GO_BACK_BUTTON_INDEX], XPM_8_8_8, 0, 0);
    goBackMainMenuSprites[0] = buttonNormal;
    res->goBackMainMenuButton = buttonNew(GO_BACK_POS.x, GO_BACK_POS.y, PAUSE_MENU_ICON_SIZE.x, PAUSE_MENU_ICON_SIZE.y, animatedSpriteNew(BUTTONS_FRAME_SPEED, goBackMainMenuSprites, 1, NORMAL), goBackMainMenuFn, basicButtonHoverFn);

    return res;
}

// Main Menu
static void newGameFn(Button* self) {
    pressedNewGame = true;
}

static void mainMenuButtonsHandler(GameState* state) {
    Vec2d mousePos = {cursorGetX(getCursor()), cursorGetY(getCursor())};
    bool leftClick = cursorIsLeftButtonPressedDown(getCursor());

    if (boxIsCollidingWithPoint(buttonGetBoxCollider(state->newGameButton), mousePos)) {
        if (leftClick) buttonClick(state->newGameButton);
        buttonHover(state->newGameButton);

    } else {
        buttonReset(state->newGameButton);
    }

    if (boxIsCollidingWithPoint(buttonGetBoxCollider(state->closeGameButton), mousePos)) {
        if (leftClick) buttonClick(state->closeGameButton);
        buttonHover(state->closeGameButton);

    } else {
        buttonReset(state->closeGameButton);
    }
}

static void mainMenuAct(GameState* state, Terminal* terminal, float deltaTime) {
    drawSpriteXPM(state->logo, terminal, LOGO_POSITION.x, LOGO_POSITION.y, false);
    buttonDraw(state->newGameButton, terminal, deltaTime, false);
    buttonDraw(state->closeGameButton, terminal, deltaTime, false);
    mainMenuButtonsHandler(state);
}

static void mainMenuFree(GameState* state) {
    free(state->logo);
    buttonFree(state->newGameButton);
    buttonFree(state->closeGameButton);
    free(state);
}

static GameState* mainMenuTransition(GameState* state, Terminal* terminal) {
    GameState* newState = state;
    // Wait for the animation to finish first - if there is one
    if (buttonIsFinished(state->newGameButton) && pressedNewGame) {
        newState = runningGameNew(terminal);
        resetBools();

    } else if (buttonIsFinished(state->closeGameButton) && pressedExit) {
        newState = NULL;
        resetBools();
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
    SpriteXPM** closeGameButtons = malloc(sizeof(SpriteXPM*) * EXIT_BUTTON_FRAMES);
    res->logo = newSpriteXPM((xpm_map_t)mainMenuXPM[LOGO], XPM_8_8_8, 0, 0);

    // New Game Button
    SpriteXPM* buttonNormal = newSpriteXPM((xpm_map_t)mainMenuXPM[NEW_GAME_BUTTON_NORMAL], XPM_8_8_8, 0, 0);
    SpriteXPM* buttonPressed = newSpriteXPM((xpm_map_t)mainMenuXPM[NEW_GAME_BUTTON_PRESSED], XPM_8_8_8, 0, 0);
    newGameButtons[0] = buttonNormal;
    newGameButtons[1] = buttonPressed;

    res->newGameButton = buttonNew(NEW_GAME_BUTTON_POS.x, NEW_GAME_BUTTON_POS.y, NEW_GAME_BUTTON_SIZE.x, NEW_GAME_BUTTON_SIZE.y, animatedSpriteNew(BUTTONS_FRAME_SPEED, newGameButtons, NEW_GAME_BUTTON_FRAMES, NORMAL), newGameFn, basicButtonHoverFn);

    // Exit Button
    buttonNormal = newSpriteXPM((xpm_map_t)mainMenuXPM[EXIT_BUTTON_NORMAL], XPM_8_8_8, 0, 0);
    buttonPressed = newSpriteXPM((xpm_map_t)mainMenuXPM[EXIT_BUTTON_PRESSED], XPM_8_8_8, 0, 0);
    closeGameButtons[0] = buttonNormal;
    closeGameButtons[1] = buttonPressed;
    res->closeGameButton = buttonNew(EXIT_BUTTON_POS.x, EXIT_BUTTON_POS.y, EXIT_BUTTON_SIZE.x, EXIT_BUTTON_SIZE.y, animatedSpriteNew(BUTTONS_FRAME_SPEED, closeGameButtons, EXIT_BUTTON_FRAMES, NORMAL), exitGameFn, basicButtonHoverFn);

    clearEmptyBuffer(terminal);
    return res;
}
