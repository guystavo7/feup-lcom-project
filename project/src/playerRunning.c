#include "./../include/player/playerRunning.h"
#include "./../include/player/playerIdle.h"

void runningAct(PlayerState* state, PlayerModel* model, Terminal* terminal, Arena* arena, float deltaTime) {
	Vec2d dir = {playerModelGetDirX(model), playerModelGetDirY(model)};
	float velocityX = playerModelGetVelocityX(model);
	float velocityY = playerModelGetVelocityY(model);
	float maxVelocity = playerModelGetMaxVelocity(model);

	RigidBody* rb = playerModelGetRigidBody(model);

	velocityX = maxVelocity * dir.x;
	velocityY = maxVelocity * dir.y;

	playerModelSetVelocityX(model, velocityX);
	playerModelSetVelocityY(model, velocityY);
    rigidBodySetVelocity(rb, velocityX, velocityY);
}

PlayerState* runningTransition(PlayerState* state, PlayerModel* model) {
	Vec2d dir = {playerModelGetDirX(model), playerModelGetDirY(model)};
	if (dir.x == 0 && dir.y == 0) return getIdleState();
    return state;
}

void runningOnEnter(PlayerState* state, PlayerModel* model, Terminal* terminal, Arena* arena, float deltaTime) {
	playerModelSetAnimatedSprite(model, playerModelGetRunningSprite(model));
}

PlayerState* getRunningState() {
	PlayerState* res = malloc(sizeof(PlayerState));
	res->act = runningAct;
	res->transition = runningTransition;
	res->onEnter = runningOnEnter;

	return res;
}
