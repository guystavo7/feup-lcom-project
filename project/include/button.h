#ifndef BUTTON
#define BUTTON

#include "../../lab5/spriteXMP.h"
#include "../../lab5/terminal.h"
#include "components/boxCollider.h"
#include "components/particle.h"

#include <stdbool.h>
#include <stdint.h>

typedef struct Button Button;

typedef void (*ButtonClickFn)(Button* self);
typedef void (*ButtonHoverFn)(Button* self);

Button* buttonNew(int x, int y, int width, int height, Particle* sprite, ButtonClickFn onClick, ButtonHoverFn onHover);
void buttonFree(Button* b);
void buttonDraw(Button* b, Terminal* t, float deltaTime, bool isFlipped);

void buttonClick(Button* button);
void buttonHover(Button* button);

void buttonReset(Button* button);

BoxCollider* buttonGetBoxCollider(Button* button);

void basicButtonHoverFn(Button* button);

#endif
