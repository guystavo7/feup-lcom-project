#include "./../include/player/playerIdle.h"
#include "./../include/player/playerRunning.h"

void idleAct(PlayerState* state, PlayerModel* model, Terminal* terminal, Arena* arena, float deltaTime) {
	playerModelSetVelocityX(model, 0);
	playerModelSetVelocityY(model, 0);
    rigidBodySetVelocity(playerModelGetRigidBody(model), 0, 0);
}

PlayerState* idleTransition(PlayerState* state, PlayerModel* model) {
	Vec2d dir = {playerModelGetDirX(model), playerModelGetDirY(model)};
	if (dir.x != 0 || dir.y != 0) return getRunningState();
    return state;
}

void idleOnEnter(PlayerState* state, PlayerModel* model, Terminal* terminal, Arena* arena, float deltaTime) {
	playerModelSetAnimatedSprite(model, playerModelGetIdleSprite(model));
}

PlayerState* getIdleState() {
	PlayerState* res = malloc(sizeof(PlayerState));
	res->act = idleAct;
	res->transition = idleTransition;
	res->onEnter = idleOnEnter;
	return res;
}
