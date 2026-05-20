#ifndef PLAYER_CONTROLLER
#define PLAYER_CONTROLLER

#include "playerModel.h"

typedef struct PlayerController PlayerController;

PlayerController* playerControllerNew(PlayerModel* model);
void playerControllerFree(PlayerController* controller);

void playerControllerUpdate(PlayerController* controller, Terminal* terminal, Arena* arena, float deltaTime);
 
#endif
