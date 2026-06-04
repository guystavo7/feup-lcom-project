#ifndef NUMBERS
#define NUMBERS

#include "../../../lab5/terminal.h"
#include "../../../lab5/spriteXMP.h"
#include "../arena.h"
#include "../utils/vec2d.h"

typedef struct Number Number;

Number* numberNew(int n);
void numberFree(Number* number);
void numberUpdate(Number* number, Vec2d position, Terminal* terminal);

int numberGetNumber(Number* number);
void numberSetNumber(Number* number, int n);

#endif
