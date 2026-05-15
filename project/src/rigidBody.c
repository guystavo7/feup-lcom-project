#include "../include/rigidBody.h"
#include "../include/boxCollider.h"
#include <stdlib.h>

struct RigidBody {
    int x;
    int y;

    int velocityX;
    int velocityY;
    BoxCollider* collider;
};

RigidBody* rigidBodyNew(int x, int y, int width, int height, int tag) {
    RigidBody* res = malloc(sizeof(RigidBody));
    res->x = x;
    res->y = y;
    res->velocityX = 0;
    res->velocityY = 0;
    res->collider = boxColliderNew(x, y, width, height, tag);

    return res;
}

void rigidBodyFree(RigidBody* rb) {
    free(rb->collider);
    free(rb);
}

BoxCollider* rigidBodyGetBoxCollider(RigidBody* rb) {
    return rb->collider;
}

static void moveX(RigidBody* rb, Arena* arena, float deltaTime) {
    int width = boxColliderGetWidth(rb->collider);
    rb->x += rb->velocityX * deltaTime;
	boxColliderSetX(rb->collider, rb->x);

    BoxCollider* colliders[10];
    int n = arenaDetectCollisions(arena, rb->collider, 0, colliders);

    for (int i = 0; i < n; i++) {
        if (rb->velocityX > 0 && (rb->x + width) > boxColliderGetX(colliders[i])) {
            rb->x = boxColliderGetX(colliders[i]) - width;

        } else if (rb->velocityX < 0 && rb->x < (boxColliderGetX(colliders[i]) + boxColliderGetWidth(colliders[i]))) {
            rb->x = boxColliderGetX(colliders[i]) + boxColliderGetWidth(colliders[i]);
        }
        rb->velocityX = 0;
    }

	boxColliderSetX(rb->collider, rb->x);
}

static void moveY(RigidBody* rb, Arena* arena, float deltaTime) {
    int height = boxColliderGetHeight(rb->collider);
    rb->y += rb->velocityY * deltaTime;
	boxColliderSetY(rb->collider, rb->y);

    BoxCollider* colliders[10];
    int n = arenaDetectCollisions(arena, rb->collider, 0, colliders);

    for (int i = 0; i < n; i++) {
        if (rb->velocityY > 0 && (rb->y + height) > boxColliderGetY(colliders[i])) {
            rb->y = boxColliderGetY(colliders[i]) - height;

        } else if (rb->velocityY < 0 && rb->y < (boxColliderGetY(colliders[i]) + boxColliderGetHeight(colliders[i]))) {
            rb->y = boxColliderGetY(colliders[i]) + boxColliderGetHeight(colliders[i]);
        }

        rb->velocityY = 0;
    }

	boxColliderSetY(rb->collider, rb->y);
}

void rigidBodyUpdate(RigidBody* rb, Arena* arena, float deltaTime) {
    moveX(rb, arena, deltaTime);
    moveY(rb, arena, deltaTime);
}

int rigidBodyGetX(RigidBody* rb) {
    return rb->x;
}

int rigidBodyGetY(RigidBody* rb) {
    return rb->y;
}

void rigidBodySetVelocity(RigidBody* rb, int xSpeed, int ySpeed) {
    rb->velocityX = xSpeed;
    rb->velocityY = ySpeed;
}
