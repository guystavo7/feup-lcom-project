#include "terminal.h"

struct Terminal {
    char* terminalMem;

    vbe_mode_info_t vmi;
    struct minix_mem_range mr;

    unsigned int bytesPerPixel;
    unsigned long vram_size;
};

Terminal* newTerminal(uint16_t mode) {
    char* terminalMem;
    vbe_mode_info_t vmi;

    if (vbe_get_mode_info(mode, &vmi)) {
        printf("Error on getting mode\n");
        return NULL;
    }

    unsigned bytesPerPixel = vmi.BitsPerPixel / 8;
    if (vmi.BitsPerPixel % 8) bytesPerPixel++;

    unsigned long vram_size = vmi.XResolution * vmi.YResolution * bytesPerPixel;

    struct minix_mem_range mr;
    mr.mr_base = (phys_bytes)vmi.PhysBasePtr;
    mr.mr_limit = mr.mr_base + vram_size;

    int r;
    if ((r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))) {
        printf("Error on sys_privctl\n");
        return NULL;
    }

    if ((terminalMem = vm_map_phys(SELF, (void*)mr.mr_base, vram_size)) == MAP_FAILED) {
        printf("Error on vm_map_phys\n");
        return NULL;
    }

    if (enterMode(mode)) {
        printf("Error on entering mode\n");
        return NULL;
    }

    Terminal* res = (Terminal*)malloc(sizeof(Terminal));
    res->vmi = vmi;
    res->mr = mr;
    res->terminalMem = terminalMem;
    res->bytesPerPixel = bytesPerPixel;
    res->vram_size = vram_size;

    return res;
}

void drawPixel(Terminal* terminal, uint16_t x, uint16_t y, uint32_t color) {
    if (y < 0 || y >= terminal->vmi.YResolution || x < 0 || x >= terminal->vmi.XResolution) return;

    long currentX = x * terminal->bytesPerPixel;
    long offset;

    for (int i = 0; i < (int)terminal->bytesPerPixel; i++) {
        uint8_t c = (color >> (8 * i)) & 0xFF;
        offset = currentX + y * terminal->vmi.XResolution * terminal->bytesPerPixel;
        terminal->terminalMem[offset + i] = c;
    }
}

void drawLine(Terminal* terminal, uint16_t x, uint16_t y, uint16_t width, uint32_t color) {
    for (int i = x; i < x + width; i++) {
        drawPixel(terminal, i, y, color);
    }
}

void freeTerminal(Terminal* terminal) {
    free(terminal);
}

unsigned int getBytesPerPixel(Terminal* terminal) {
    return terminal->bytesPerPixel;
}
