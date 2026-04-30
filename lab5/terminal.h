#ifndef TERMINAL
#define TERMINAL

#include "terminalUtils.h"
#include <lcom/lcf.h>

typedef struct Terminal Terminal;

Terminal* newTerminal(uint16_t mode);
void drawPixel(Terminal* terminal, uint16_t x, uint16_t y, uint32_t color);
void drawLine(Terminal* terminal, uint16_t x, uint16_t y, uint16_t width, uint32_t color);

void freeTerminal(Terminal* terminal);

unsigned int getBytesPerPixel(Terminal* terminal);
void refreshTerminal(Terminal* terminal);

#endif
