#include <lcom/lcf.h>

#include <stdint.h>

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
