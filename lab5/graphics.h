#include <lcom/lcf.h>

#include <stdint.h>

extern char *video_mem;

extern vbe_mode_info_t vmi;
extern unsigned bytes_per_pixel;


int setGraphicsMode(uint16_t mode);

int setTextMode(void);

int vg_map_memory(uint16_t mode);

int (writeCmd)(uint8_t cmd);

int startGraphics(uint16_t mode);

int vg_draw_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t color);

int gfx_draw_hline(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

int gfx_draw_pixel(uint16_t x, uint16_t y, uint32_t color);
