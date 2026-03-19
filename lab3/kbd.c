#include <lcom/lcf.h>
#include <stdint.h>
#include "i8254.h"

static uint16_t scancode;

void kbc_ih() {
  uint8_t st;
  if(sys_inb(0x64, &st)) return;
  if (st & (KBC_PARITY_ERROR | KBC_TIMEOUT_ERROR)) return;
  if (!(st & bit(0))) return;
  if (sys_outb(0x60, &scancode)) return;
}