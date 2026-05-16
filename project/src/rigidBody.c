#include "../include/rigidBody.h"
#include "../include/boxCollider.h"
#include "../include/vec2d.h"
#include <stdlib.h>

struct RigidBody {
	Vec2d velocity;
};

RigidBody* rigidBodyNew(Vec2d pos, Vec2d offset, Vec2d size, int tag) {
    RigidBody* res = malloc(sizeof(RigidBody));
    res->velocity.x = 0;
    res->velocity.y = 0;

    return res;
}

void rigidBodyFree(RigidBody* rb) {
    free(rb);
}

static int moveX(RigidBody* rb, int x, Arena* arena, BoxCollider* hitBox, float deltaTime) {
    int width = boxColliderGetWidth(hitBox);
    x += rb->velocity.x * deltaTime;
	boxColliderSetX(hitBox, x);

    BoxCollider* colliders[10];
    int n = arenaDetectCollisions(arena, hitBox, 0, colliders);

    for (int i = 0; i < n; i++) {
        if (rb->velocity.x > 0 && (x + width) > boxColliderGetX(colliders[i])) {
            x = boxColliderGetX(colliders[i]) - width;

        } else if (rb->velocity.x < 0 && x < (boxColliderGetX(colliders[i]) + boxColliderGetWidth(colliders[i]))) {
            x = boxColliderGetX(colliders[i]) + boxColliderGetWidth(colliders[i]);
        }
        rb->velocity.x = 0;
    }

	boxColliderSetX(hitBox, x);
	return x;
}

static int moveY(RigidBody* rb, int y, Arena* arena, BoxCollider* hitBox, float deltaTime) {
    int height = boxColliderGetHeight(hitBox);
    y += rb->velocity.y * deltaTime;
	boxColliderSetY(hitBox, y);

    BoxCollider* colliders[10];
    int n = arenaDetectCollisions(arena, hitBox, 0, colliders);

    for (int i = 0; i < n; i++) {
        if (rb->velocity.y > 0 && (y + height) > boxColliderGetY(colliders[i])) {
            y = boxColliderGetY(colliders[i]) - height;

        } else if (rb->velocity.y < 0 && y < (boxColliderGetY(colliders[i]) + boxColliderGetHeight(colliders[i]))) {
            y = boxColliderGetY(colliders[i]) + boxColliderGetHeight(colliders[i]);
        }

        rb->velocity.y = 0;
    }

	boxColliderSetY(hitBox, y);
	return y;
}

void rigidBodyUpdate(RigidBody* rb, Vec2d* pos, Arena* arena, BoxCollider* hitBox, float deltaTime) {
    pos->x = moveX(rb, pos->x, arena, hitBox, deltaTime);
    pos->y = moveY(rb, pos->y, arena, hitBox, deltaTime);
}

void rigidBodySetVelocity(RigidBody* rb, int xSpeed, int ySpeed) {
    rb->velocity.x = xSpeed;
    rb->velocity.y = ySpeed;
}
