#ifndef TERMINAL
#define TERMINAL

#include "terminalUtils.h"

static char* terminalMem;

static vbe_mode_info_t vmi;
static struct minix_mem_range mr;

static unsigned bytesPerPixel; 
static unsigned long vram_size;

int newTerminal(uint16_t mode);
void drawPixel(uint16_t x, uint16_t y, uint32_t color);
void drawLine(uint16_t x, uint16_t y, uint16_t width, uint32_t color);

#endif
