#include "./../utils/vec2d.h"
#include "./../arena.h"
#include "./../fruit.h"
#include "playerModel.h"
#include "playerController.h"

typedef struct Player Player;

Player* playerNew(Vec2d pos);
void playerFree(Player* player);
void playerUpdate(Player* player, Terminal* terminal, Arena* arena, float deltaTime);

PlayerModel* playerGetPlayerModel(Player* player);
PlayerController* playerGetPlayerController(Player* player);

void playerAddFruit(Player* player, FruitKind kind);
void playerPrintFruitInventory(const Player* player);
