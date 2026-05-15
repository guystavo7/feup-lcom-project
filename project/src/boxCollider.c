#include "../include/boxCollider.h"
#include <stdlib.h>

struct BoxCollider {
    int x;
    int y;
    int width;
    int height;
    int tag;
};

BoxCollider* boxColliderNew(int x, int y, int width, int height, int tag) {
    BoxCollider* res = malloc(sizeof(BoxCollider));
    res->x = x;
    res->y = y;
    res->width = width;
    res->height = height;
    res->tag = tag;

    return res;
}

void boxColliderFree(BoxCollider* boxCollider) {
    free(boxCollider);
}

bool boxIsCollidingWithBox(BoxCollider* b1, BoxCollider* b2) {
    return b1->x < b2->x + b2->width && b1->x + b1->width > b2->x && b1->y < b2->y + b2->height && b1->y + b1->height > b2->y;
}

bool boxIsCollidingWithPoint(BoxCollider* b, int x, int y) {
    return b->x >= x && b->x + b->width <= x && b->y >= y && b->y + b->height <= y;
}

void boxColliderUpdate(BoxCollider* b, int x, int y) {
    b->x = x;
    b->y = y;
}

void boxColliderSetX(BoxCollider* b, int x) {
    b->x = x;
}

void boxColliderSetY(BoxCollider* b, int y) {
    b->y = y;
}

int boxColliderGetTag(BoxCollider* b) {
    return b->tag;
}

int boxColliderGetHeight(BoxCollider* b) {
    return b->height;
}

int boxColliderGetWidth(BoxCollider* b) {
    return b->width;
}

int boxColliderGetX(BoxCollider* b) {
    return b->x;
}

int boxColliderGetY(BoxCollider* b) {
    return b->y;
}
