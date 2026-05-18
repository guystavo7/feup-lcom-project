#include "../include/player.h"
#include "../include/boxCollider.h"
#include "../include/inputHandler.h"
#include "../include/rigidBody.h"
#include "../resources/warrior.h"

#include <stdlib.h>

struct Player {
    Vec2d pos;
    Vec2d size;
    Vec2d velocity;
    Vec2d dir;

    AnimatedSprite* sprite;
    BoxCollider* hitBox;
    RigidBody* rb;
    int maxVelocity;
};

Player* playerNew(Vec2d pos) {
    Player* res = malloc(sizeof(Player));
    res->pos.x = pos.x;
    res->pos.y = pos.y;

    res->size.x = 16;
    res->size.y = 16;

    res->maxVelocity = 200;
	res->dir.x = 0;
	res->dir.y = 0;

    Vec2d offset = {0, 0};
    res->hitBox = boxColliderNew(pos, offset, res->size, 0);
    res->rb = rigidBodyNew(offset, res->size, 0);

	int n = 6;
    SpriteXPM** frames = malloc(n * sizeof(SpriteXPM*));

	for (int i = 8; i < 8 + n; i++) {
		frames[i - 8] = newSpriteXPM((xpm_map_t)all_xpms[i], XPM_8_8_8, offset.x, offset.y);
	}

    res->sprite = animatedSpriteNew(10.0f, frames, n);
	return res;
}

void playerFree(Player* player) {
    free(player->hitBox);
    free(player->rb);
    animatedSpriteFree(player->sprite);
    free(player->sprite);
    free(player);
}

static void getInput(Player* player) {
	player->dir.x = 0;
	player->dir.y = 0;

    if (getKeyPressed(getInputHandler(), KEYCODE_W)) player->dir.y = -1.0;
    if (getKeyPressed(getInputHandler(), KEYCODE_S)) player->dir.y = 1.0;
    if (getKeyPressed(getInputHandler(), KEYCODE_A)) player->dir.x = -1.0;
    if (getKeyPressed(getInputHandler(), KEYCODE_D)) player->dir.x = 1.0;
	vec2dNormalize(&player->dir);
}

static void updateVelocity(Player* player, float deltaTime) {
	player->velocity.x = player->maxVelocity * player->dir.x;
	player->velocity.y = player->maxVelocity * player->dir.y;

	rigidBodySetVelocity(player->rb, player->velocity.x, player->velocity.y);
}

void playerUpdate(Player* player, Terminal* terminal, Arena* arena, float deltaTime) {
    getInput(player);
	updateVelocity(player, deltaTime);

    rigidBodyUpdate(player->rb, &player->pos, arena, player->hitBox, deltaTime);
    animatedSpriteUpdate(player->pos.x, player->pos.y, player->sprite, terminal, deltaTime);
}
