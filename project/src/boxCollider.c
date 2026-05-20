#include "../include/components/boxCollider.h"
#include "../include/utils/vec2d.h"
#include <stdlib.h>

struct BoxCollider {
    Vec2d pos;
    Vec2d offset;
    Vec2d size;

    int tag;
};

BoxCollider* boxColliderNew(Vec2d pos, Vec2d offset, Vec2d size, int tag) {
    BoxCollider* res = malloc(sizeof(BoxCollider));
    res->pos.x = pos.x;
    res->pos.y = pos.y;

    res->offset.x = offset.x;
    res->offset.y = offset.y;

    res->size.x = size.x;
    res->size.y = size.y;
    res->tag = tag;

    return res;
}

void boxColliderFree(BoxCollider* boxCollider) {
    free(boxCollider);
}

bool boxIsCollidingWithBox(BoxCollider* b1, BoxCollider* b2) {
    float x1 = b1->pos.x + b1->offset.x;
    float y1 = b1->pos.y + b1->offset.y;

    float x2 = b2->pos.x + b2->offset.x;
    float y2 = b2->pos.y + b2->offset.y;

    return x1 < x2 + b2->size.x && x1 + b1->size.x > x2 && y1 < y2 + b2->size.y && y1 + b1->size.y > y2;
}

bool boxIsCollidingWithPoint(BoxCollider* b, Vec2d pos) {
    return b->pos.x >= pos.x && b->pos.x + b->size.x <= pos.x && b->pos.y >= pos.y && b->pos.y + b->size.y <= pos.y;
}

void boxColliderUpdate(BoxCollider* b, int x, int y) {
    b->pos.x = x;
    b->pos.y = y;
}

void boxColliderSetX(BoxCollider* b, int x) {
    b->pos.x = x;
}

void boxColliderSetY(BoxCollider* b, int y) {
    b->pos.y = y;
}

void boxColliderSetPos(BoxCollider *b, Vec2d pos) {
	b->pos.x = pos.x;
	b->pos.y = pos.y;
}

int boxColliderGetTag(BoxCollider* b) {
    return b->tag;
}

float boxColliderGetHeight(BoxCollider* b) {
    return b->size.y;
}

float boxColliderGetWidth(BoxCollider* b) {
    return b->size.x;
}

float boxColliderGetX(BoxCollider* b) {
    return b->pos.x;
}

float boxColliderGetY(BoxCollider* b) {
    return b->pos.y;
}

float boxColliderGetOffsetX(BoxCollider* b) {
    return b->offset.x;
}

float boxColliderGetOffsetY(BoxCollider *b) {
	return b->offset.y;
}
