#include "../include/button.h"
#include "../include/components/boxCollider.h"
#include "../include/utils/vec2d.h"
#include "../include/data.h"

#include <stdlib.h>

struct Button {
	Particle* sprite;
    BoxCollider* bounds;
    int x;
    int y;

    ButtonClickFn onClick;
	ButtonHoverFn onHover;

	bool isPressed;
	bool isHovering;
};

Button* buttonNew(int x, int y, int width, int height, Particle* sprite, ButtonClickFn onClick, ButtonHoverFn onHover) {
    Button* b = malloc(sizeof(Button));
	b->sprite = sprite;
    b->x = x;
    b->y = y;
    b->onClick = onClick;
	b->onHover = onHover;

	b->isPressed = false;
	b->isHovering = false;


	Vec2d pos = {x, y};
	Vec2d size = {width, height};
    b->bounds = boxColliderNew(pos, EMPTY_VEC2D, size, UI_TAG);

    return b;
}

void buttonFree(Button* b) {
    if (!b) return;
	particleFree(b->sprite);
    boxColliderFree(b->bounds);
    free(b);
}

void buttonDraw(Button* b, Terminal* t, float deltaTime, bool isFlipped) {
	particleUpdate(b->x, b->y, b->sprite, t, deltaTime, isFlipped);
}

void buttonClick(Button* button) {
	if (button->isPressed) return;
	button->onClick(button);
	button->isPressed = true;
}

void buttonHover(Button* button) {
	button->onHover(button);
}

void buttonReset(Button* button) {
	button->isPressed = false;
	button->isHovering = false;

	particleReset(button->sprite);
}

BoxCollider* buttonGetBoxCollider(Button* button) {
	return button->bounds;
}

// Button Hover
void basicButtonHoverFn(Button* button) {
	if (button->isHovering) return;

	particleStart(button->sprite);
	button->isHovering = true;
}
