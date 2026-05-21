#include "../include/player/playerController.h"
#include "../include/player/playerIdle.h"
#include "../include/player/playerState.h"
#include "../include/utils/inputHandler.h"
#include <stdint.h>

struct PlayerController {
    PlayerModel* model;
    PlayerState* state;
};

PlayerController* playerControllerNew(PlayerModel* model) {
    PlayerController* res = malloc(sizeof(PlayerController));
    res->model = model;
    res->state = getIdleState();

    return res;
}

void playerControllerFree(PlayerController* controller) {
    free(controller->state);
    free(controller);
}

static void getInput(PlayerController* controller) {
    Vec2d dir = {0, 0};
    PlayerModel* model = controller->model;

    if (getKeyPressed(getInputHandler(), KEYCODE_W)) dir.y = -1.0f;
    if (getKeyPressed(getInputHandler(), KEYCODE_S)) dir.y = 1.0f;
    if (getKeyPressed(getInputHandler(), KEYCODE_A)) dir.x = -1.0f;
    if (getKeyPressed(getInputHandler(), KEYCODE_D)) dir.x = 1.0f;

    vec2dNormalize(&dir);
    playerModelSetDir(model, dir);
}

static void handleState(PlayerController* controller, Terminal* terminal, Arena* arena, float deltaTime) {
    PlayerState* state = controller->state;
    state->act(state, controller->model, terminal, arena, deltaTime);
    PlayerState* newState = state->transition(state, controller->model);

    if (state != newState) {
        free(state);
        controller->state = newState;
        newState->onEnter(newState, controller->model, terminal, arena, deltaTime);
    }
}

static void handlePhysics(PlayerController* controller, Arena* arena, float deltaTime) {
    PlayerModel* model = controller->model;
    RigidBody* rb = playerModelGetRigidBody(model);
    BoxCollider* hitBox = playerModelGetHitBox(model);
    Vec2d pos = {playerModelGetX(model), playerModelGetY(model)};

    rigidBodyUpdate(rb, &pos, arena, hitBox, deltaTime);
    playerModelSetPos(model, pos);
}

static void handleVisual(PlayerController* controller, Terminal* terminal, float deltaTime) {
    PlayerModel* model = controller->model;
    Vec2d pos = {playerModelGetX(model), playerModelGetY(model)};

    AnimatedSprite* sprite = playerModelGetAnimatedSprite(model);
    float dirX = playerModelGetDirX(model);
    bool isFlipped = playerModelGetIsFlipped(model);

    if ((dirX > 0 && isFlipped) || (dirX < 0 && !isFlipped)) {
		playerModelSetIsFlipped(model, !isFlipped);
	}

    animatedSpriteUpdate(pos.x, pos.y, sprite, terminal, deltaTime, playerModelGetIsFlipped(model));
}

void playerControllerUpdate(PlayerController* controller, Terminal* terminal, Arena* arena, float deltaTime) {
    getInput(controller);
    handleState(controller, terminal, arena, deltaTime);
    handlePhysics(controller, arena, deltaTime);
    handleVisual(controller, terminal, deltaTime);
}
