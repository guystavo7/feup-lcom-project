#include <lcom/lcf.h>
#include <stdint.h>
#include "i8254.h"
#include "utils.h"
#include "kbd.h"

int hookId = 1;
uint16_t scancode = 0;
uint16_t getCurrentScancode(void) {
  return scancode;
}

int getCurrentHookId(void) {
  return hookId;
}

int (kbd_subscribe_int)(uint8_t *bit_no) {
  hookId = *bit_no;
  *bit_no = (uint8_t) hookId;
  if (sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hookId) != 0)
    return 1;
  return 0;
}

int (kbd_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hookId) != 0) return 1;
  return 0;
}

void (kbc_ih)() {
  uint8_t st, data;
  if (util_sys_in8b(0x64, &st) != 0) return;
  if (st & (KBD_PARITY_ERROR | KBD_TIMEOUT_ERROR)) return;
  if (!(st & BIT(0))) return; /* output buffer empty */
  if (util_sys_in8b(0x60, &data) != 0) return;

  static uint8_t first = 0;
  if (data == 0xE0) {
    first = data;
    return;
  }
  if (first == 0xE0) {
    scancode = ((uint16_t)first << 8) | data;
    first = 0;
  }
  else {
    scancode = data;
  }
}
