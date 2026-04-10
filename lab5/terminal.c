#include "terminal.h"
#include <stdint.h>

int newTerminal(uint16_t mode) {
    if (vbe_get_mode_info(mode, &vmi)) {
        printf("Error on getting mode\n");
        return -1;
    }

    bytesPerPixel = vmi.BitsPerPixel / 8;
    if (vmi.BitsPerPixel % 8) bytesPerPixel++;

    mr.mr_base = (phys_bytes)vmi.PhysBasePtr;
    mr.mr_limit = mr.mr_base + vmi.XResolution * vmi.YResolution * bytesPerPixel;

    int r;
    if ((r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))) {
        printf("Error on sys_privctl\n");
        return -1;
    }

    if ((terminalMem = vm_map_phys(SELF, (void*)mr.mr_base, vmi.XResolution * vmi.YResolution)) == MAP_FAILED) {
        printf("Error on vm_map_phys\n");
        return -1;
    }

    if (enterMode(mode)) {
        printf("Error on entering mode\n");
        return -1;
    }

    return 0;
}

void drawPixel(uint16_t x, uint16_t y, uint32_t color) {
	uint16_t currentX = x * bytesPerPixel;

	for (int i = 0; i < (int) bytesPerPixel; i++) {
		uint8_t c = color >> (8 * i);
		terminalMem[currentX + i + y * vmi.XResolution * (int) bytesPerPixel] = c; 
	}
}

void drawLine(uint16_t x, uint16_t y, uint16_t width, uint32_t color) {
    for (int i = x; i < x + width; i++) {
        drawPixel(i, y, color);
    }
}
