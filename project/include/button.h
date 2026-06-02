#ifndef BUTTON
#define BUTTON

#include "../../lab5/spriteXMP.h"
#include "../../lab5/terminal.h"

#include <stdbool.h>
#include <stdint.h>

typedef struct Button Button;

typedef void (*ButtonClickFn)(Button* self);

Button* buttonNew(xpm_map_t xpm, int x, int y, ButtonClickFn onClick);
void buttonFree(Button* b);
void buttonDraw(Button* b, Terminal* t);

bool buttonContainsPoint(Button* b, int px, int py);

#endif
