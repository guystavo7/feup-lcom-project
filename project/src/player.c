#include "../include/player/player.h"

#include <stdio.h>
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
	for (int i = 0; i < FRUIT_KIND_COUNT; i++) res->fruitCount[i] = 0;

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

void playerPrintFruitInventory(const Player* player) {
	unsigned total = 0;
	for (int i = 0; i < FRUIT_KIND_COUNT; i++) total += player->fruitCount[i];
	printf(
	    "Frutas: banana=%u abacate=%u cereja=%u cenoura=%u (total=%u)\n",
	    player->fruitCount[FRUIT_BANANA],
	    player->fruitCount[FRUIT_ABACATE],
	    player->fruitCount[FRUIT_CEREJA],
	    player->fruitCount[FRUIT_CENOURA],
	    total);
	fflush(stdout);
}
