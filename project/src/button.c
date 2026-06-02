#include "../include/button.h"
#include "../include/components/boxCollider.h"
#include "../include/utils/vec2d.h"
#include "../include/data.h"

#include <stdlib.h>

#define UI_MAX_BUTTONS 8
#define UI_MAX_POPUPS 4

struct Button {
    SpriteXPM* sprite;
    BoxCollider* bounds;
    int x;
    int y;
    ButtonClickFn onClick;
};

static BoxCollider* boundsFromSprite(SpriteXPM* sprite, int x, int y) {
    Vec2d pos = {x, y};
    Vec2d size = {getSpriteWidth(sprite), getSpriteHeight(sprite)};
    return boxColliderNew(pos, EMPTY_VEC2D, size, 0);
}

Button* buttonNew(xpm_map_t xpm, int x, int y, ButtonClickFn onClick) {
    Button* b = malloc(sizeof(Button));
    b->sprite = newSpriteXPM(xpm, XPM_8_8_8, 0, 0);
    b->x = x;
    b->y = y;
    b->onClick = onClick;
    b->bounds = boundsFromSprite(b->sprite, x, y);
    return b;
}

void buttonFree(Button* b) {
    if (!b) return;
    freeSpriteXPM(b->sprite);
    boxColliderFree(b->bounds);
    free(b);
}

void buttonDraw(Button* b, Terminal* t) {
    drawSpriteXPM(b->sprite, t, b->x, b->y, false);
}
