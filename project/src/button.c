#include "../include/button.h"
#include "../include/boxCollider.h"
#include "../include/vec2d.h"

#include <stdlib.h>

#define UI_MAX_BUTTONS 8
#define UI_MAX_POPUPS 4

struct Button {
    SpriteXPM* sprite;
    BoxCollider* bounds;
    uint16_t x;
    uint16_t y;
    ButtonClickFn onClick;
};

struct Popup {
    SpriteXPM* sprite;
    BoxCollider* bounds;
    uint16_t x;
    uint16_t y;
    bool visible;
};

struct UIManager {
    Button* buttons[UI_MAX_BUTTONS];
    unsigned int buttonCount;
    Popup* popups[UI_MAX_POPUPS];
    unsigned int popupCount;
};

static BoxCollider* boundsFromSprite(SpriteXPM* sprite, uint16_t x, uint16_t y) {
    Vec2d pos = {x, y};
    Vec2d offset = {0, 0};
    Vec2d size = {getSpriteWidth(sprite), getSpriteHeight(sprite)};
    return boxColliderNew(pos, offset, size, 0);
}

Button* buttonNew(xpm_map_t xpm, uint16_t x, uint16_t y, ButtonClickFn onClick) {
    Button* b = malloc(sizeof(Button));
    b->sprite = newSpriteXPM(xpm, XPM_8_8_8);
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
    drawSpriteXPM(b->sprite, t, b->x, b->y);
}

Popup* popupNew(xpm_map_t xpm, uint16_t x, uint16_t y) {
    Popup* p = malloc(sizeof(Popup));
    p->sprite = newSpriteXPM(xpm, XPM_8_8_8);
    p->x = x;
    p->y = y;
    p->visible = false;
    p->bounds = boundsFromSprite(p->sprite, x, y);
    return p;
}

void popupFree(Popup* p) {
    if (!p) return;
    freeSpriteXPM(p->sprite);
    boxColliderFree(p->bounds);
    free(p);
}

void popupOpen(Popup* p) {
    p->visible = true;
}

void popupClose(Popup* p) {
    p->visible = false;
}

bool popupIsOpen(Popup* p) {
    return p->visible;
}

void popupDraw(Popup* p, Terminal* t) {
    if (!p->visible) return;
    drawSpriteXPM(p->sprite, t, p->x, p->y);
}

UIManager* uiManagerNew(void) {
    UIManager* ui = malloc(sizeof(UIManager));
    ui->buttonCount = 0;
    ui->popupCount = 0;
    return ui;
}

void uiManagerFree(UIManager* ui) {
    if (!ui) return;

    for (unsigned int i = 0; i < ui->buttonCount; i++) buttonFree(ui->buttons[i]);

    for (unsigned int i = 0; i < ui->popupCount; i++) popupFree(ui->popups[i]);

    free(ui);
}

void uiManagerAddButton(UIManager* ui, Button* b) {
    if (ui->buttonCount >= UI_MAX_BUTTONS) return;
    ui->buttons[ui->buttonCount++] = b;
}

void uiManagerAddPopup(UIManager* ui, Popup* p) {
    if (ui->popupCount >= UI_MAX_POPUPS) return;
    ui->popups[ui->popupCount++] = p;
}

void uiManagerDraw(UIManager* ui, Terminal* t) {
    for (unsigned int i = 0; i < ui->buttonCount; i++) buttonDraw(ui->buttons[i], t);

    for (unsigned int i = 0; i < ui->popupCount; i++) popupDraw(ui->popups[i], t);
}

void uiManagerHandleClick(UIManager* ui, int mx, int my) {
    Vec2d point = {mx, my};

    for (unsigned int i = 0; i < ui->buttonCount; i++) {
        Button* b = ui->buttons[i];
        if (boxIsCollidingWithPoint(b->bounds, point)) {
            if (b->onClick) b->onClick(b);
            return;
        }
    }

    for (unsigned int i = 0; i < ui->popupCount; i++) {
        Popup* p = ui->popups[i];
        if (!popupIsOpen(p)) continue;

        if (boxIsCollidingWithPoint(p->bounds, point)) return;
    }

    for (unsigned int i = 0; i < ui->popupCount; i++) {
        if (popupIsOpen(ui->popups[i])) popupClose(ui->popups[i]);
    }
}
