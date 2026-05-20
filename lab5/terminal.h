#ifndef TERMINAL
#define TERMINAL

#include "terminalUtils.h"
#include <lcom/lcf.h>

typedef struct Terminal Terminal;

Terminal* newTerminal(uint16_t mode);
void drawPixel(Terminal* terminal, int x, int y, uint32_t color);
void drawLine(Terminal* terminal, int x, int y, uint16_t width, uint32_t color);

void freeTerminal(Terminal* terminal);

unsigned int getBytesPerPixel(Terminal* terminal);
void refreshTerminal(Terminal* terminal);

#endif
