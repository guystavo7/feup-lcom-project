#include "../include/player/player.h"

#include <stdlib.h>

struct Player {
	PlayerController* controller;
	PlayerModel* model;
	unsigned fruitCount[FRUIT_KIND_COUNT];
};

Player* playerNew(Vec2d pos) {
	PlayerModel* model = playerModelNew(pos);
	PlayerController* controller = playerControllerNew(model);

	Player* res = malloc(sizeof(Player));
	res->model = model;
	res->controller = controller;
	for (int i = 0; i < FRUIT_KIND_COUNT; i++){
		if(i == FRUIT_BANANA || i == FRUIT_ABACATE){
			res->fruitCount[i] = 5;
		}
		else{
			res->fruitCount[i] = 0;
		}
	};

	return res;
}

void playerFree(Player* player) {
	playerModelFree(player->model);
	playerControllerFree(player->controller);
	free(player);
}

void playerUpdate(Player* player, Terminal* terminal, Arena* arena, float deltaTime) {
	playerControllerUpdate(player->controller, terminal, arena, deltaTime);
}

PlayerModel* playerGetPlayerModel(Player* player) {
	return player->model;
}

PlayerController* playerGetPlayerController(Player* player) {
	return player->controller;
}

void playerAddFruit(Player* player, FruitKind kind) {
	if ((unsigned)kind >= (unsigned)FRUIT_KIND_COUNT) return;
	player->fruitCount[kind]++;
}

unsigned playerGetFruitCount(const Player* player, FruitKind kind) {
	if ((unsigned)kind >= (unsigned)FRUIT_KIND_COUNT) return 0;
	return player->fruitCount[kind];
}
