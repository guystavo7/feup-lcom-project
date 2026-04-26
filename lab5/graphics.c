#include <lcom/lcf.h>

#include <stdint.h>
#include "graphics.h"

char *video_mem;
vbe_mode_info_t vmi;
unsigned bytes_per_pixel;

int setGraphicsMode(uint16_t mode) {
  reg86_t reg;
  memset(&reg, 0, sizeof(reg));
  reg.intno = 0x10;
  reg.ah = 0x4F;
  reg.al = 0x02;
  reg.bx = mode | BIT(14);
  if( sys_int86(&reg) != OK ) {
    printf("sys_int86() failed \n");
    return 1;
  }
  return 0;
}

int setTextMode(void) {
  reg86_t reg;
  memset(&reg, 0, sizeof(reg));
  reg.intno = 0x10;
  reg.ah = 0x00;
  reg.al = 0x03;
  if( sys_int86(&reg) != OK ) {
    printf("sys_int86() failed \n");
    return 1;
  }
  return 0;
}

int (writeCmd)(uint8_t cmd) {
  if (!sys_outb(0x64, 0xD4)) return 1;
  if(!sys_outb(0x60, cmd)) return 1;
  return 0;
}

void*(vg_init)(uint16_t mode) {
  if(vg_map_memory(mode)) return NULL;
  if(setGraphicsMode(mode)) return NULL;
  return video_mem;
}

int vg_map_memory(uint16_t mode) {
  int r;
  struct minix_mem_range mr;
  unsigned int vram_base;
  unsigned int vram_size;

  if(vbe_get_mode_info(mode, &vmi)) return 1;

  vram_base = vmi.PhysBasePtr;
  vram_size = (vmi.BitsPerPixel/8) * vmi.XResolution * vmi.YResolution; 

  /* Grant memory mapping permissions */
  mr.mr_base = (phys_bytes) vram_base;
  mr.mr_limit = mr.mr_base + vram_size;  

  if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);

  /* Map memory */
  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

  if(video_mem == MAP_FAILED)
    panic("couldn't map video memory");

  return 0;
}

int startGraphics(uint16_t mode) {
  return vg_map_memory(mode);
}

int vg_draw_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t color) {
  for (int i = y; i < h + y; i++){ 
    if (gfx_draw_hline(x, i, w, color)) return 1;
  }
  return 0;
}

int gfx_draw_hline(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  for (int i = x; i < x + len; i++){
    if (gfx_draw_pixel(i, y, color)) return 1;
  }
  return 0;
}

int gfx_draw_pixel(uint16_t x, uint16_t y, uint32_t color){
  unsigned int bytes_per_pixel = vmi.BitsPerPixel / 8;
  unsigned int offset = (y * vmi.XResolution + x) * bytes_per_pixel;
  memcpy(&video_mem[offset], (uint8_t*)&color, bytes_per_pixel);
  return 0;
}
