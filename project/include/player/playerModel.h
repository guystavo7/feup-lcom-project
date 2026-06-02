#ifndef PLAYER_MODEL
#define PLAYER_MODEL

#include "../components/animatedSprite.h"
#include "../components/rigidBody.h"
#include "./../arena.h"
#include "./../utils/vec2d.h"
#include <stdbool.h>

typedef struct PlayerModel PlayerModel;

PlayerModel* playerModelNew(Vec2d pos);
void playerModelFree(PlayerModel* playerModel);

float playerModelGetX(PlayerModel* playerModel);
float playerModelGetY(PlayerModel* playerModel);

float playerModelGetWidth(PlayerModel* playerModel);
float playerModelGetHeight(PlayerModel* playerModel);

float playerModelGetDirX(PlayerModel* playerModel);
float playerModelGetDirY(PlayerModel* playerModel);

float playerModelGetMaxVelocity(PlayerModel* playerModel);

AnimatedSprite* playerModelGetAnimatedSprite(PlayerModel* playerModel);
BoxCollider* playerModelGetHitBox(PlayerModel* playerModel);
RigidBody* playerModelGetRigidBody(PlayerModel* playerModel);

void playerModelSetX(PlayerModel* playerModel, float x);
void playerModelSetY(PlayerModel* playerModel, float y);
void playerModelSetPos(PlayerModel* playerModel, Vec2d pos);

void playerModelSetDirX(PlayerModel* playerModel, float dirX);
void playerModelSetDirY(PlayerModel* playerModel, float dirY);
void playerModelSetDir(PlayerModel* playerModel, Vec2d dir);

float playerModelGetVelocityX(PlayerModel* playerModel);
float playerModelGetVelocityY(PlayerModel* playerModel);
void playerModelSetVelocityX(PlayerModel* playerModel, float x);
void playerModelSetVelocityY(PlayerModel* playerModel, float y);

AnimatedSprite* playerModelGetRunningSprite(PlayerModel* model);
AnimatedSprite* playerModelGetIdleSprite(PlayerModel* model);

void playerModelSetAnimatedSprite(PlayerModel* model, AnimatedSprite* sprite);

bool playerModelGetIsFlipped(PlayerModel* model);
void playerModelSetIsFlipped(PlayerModel* model, bool isFlip);

#endif
