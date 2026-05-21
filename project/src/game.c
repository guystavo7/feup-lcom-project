#include "../include/game.h"
#include "../include/animatedSprite.h"
#include "../include/arena.h"
#include "../include/inputHandler.h"
#include "../include/rigidBody.h"
#include "../include/cursorHandler.h"

#include "../resources/button.h"
#include "../resources/coinEdge.h"
#include "../resources/coinMid.h"
#include "../resources/coinMidDark.h"
#include "../resources/cursor.h"
#include "../resources/popUp.h"

static Arena* arena;
static SpriteXPM* mouse;
static RigidBody* rb;

static BoxCollider* hitBox;
static Vec2d playerPos = {100, 100};

static AnimatedSprite* playerSprite;
static UIManager* ui;
static Popup* menuPopup;

static void onMenuButtonClick(Button* self) {
    (void)self;
    popupOpen(menuPopup);
}

void startGame() {
    mouse = newSpriteXPM((xpm_map_t)cursor_xpm, XPM_8_8_8);

    Vec2d playerSize = {16, 16};
    Vec2d offset = {0, 0};
    hitBox = boxColliderNew(playerPos, offset, playerSize, 0);

    rb = rigidBodyNew(playerPos, offset, playerSize, 0);
    arena = arenaNewFromFile("/home/lcom/labs/project/resources/map1_TileLayer1.csv", 32);

    SpriteXPM** frames = malloc(3 * sizeof(SpriteXPM*));

    frames[0] = newSpriteXPM((xpm_map_t)coin_f3_xpm, XPM_8_8_8);
    frames[1] = newSpriteXPM((xpm_map_t)coin_f2_xpm, XPM_8_8_8);
    frames[2] = newSpriteXPM((xpm_map_t)coin_f4_xpm, XPM_8_8_8);

    playerSprite = animatedSpriteNew(1.0f, frames, 3);

    ui = uiManagerNew();
    menuPopup = popupNew((xpm_map_t)popup_xpm, 250, 175);
    Button* menuButton = buttonNew((xpm_map_t)button_sprite_xpm, 20, 20, onMenuButtonClick);
    uiManagerAddPopup(ui, menuPopup);
    uiManagerAddButton(ui, menuButton);
}

void exitGame() {
    uiManagerFree(ui);
    ui = NULL;
    menuPopup = NULL;
    animatedSpriteFree(playerSprite);
    freeSpriteXPM(mouse);
    rigidBodyFree(rb);
    boxColliderFree(hitBox);
    arenaFree(arena);
}

UIManager* getUIManager(void) {
    return ui;
}

void gameLoop(Terminal* terminal, float deltaTime) {
    int dirX = 0, dirY = 0;
    int playerSpeed = 400;

    if (getKeyPressed(getInputHandler(), KEYCODE_W)) dirY = -1;
    if (getKeyPressed(getInputHandler(), KEYCODE_S)) dirY = 1;
    if (getKeyPressed(getInputHandler(), KEYCODE_A)) dirX = -1;
    if (getKeyPressed(getInputHandler(), KEYCODE_D)) dirX = 1;

    rigidBodySetVelocity(rb, playerSpeed * dirX, playerSpeed * dirY);
    rigidBodyUpdate(rb, &playerPos, arena, hitBox, deltaTime);
    animatedSpriteUpdate(playerPos.x, playerPos.y, playerSprite, terminal, deltaTime);
    arenaUpdade(arena, terminal);
    uiManagerDraw(ui, terminal);
    cursorDraw(terminal, mouse);
}
