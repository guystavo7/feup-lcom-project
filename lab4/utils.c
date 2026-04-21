#include <lcom/lcf.h>

#include <stdint.h>

int(util_get_LSB_mouse)(uint16_t val, uint8_t *lsb) {
  if (lsb == NULL) return 1;
  *lsb = (uint8_t)(val & 0xFF);
  return 0;
}

int(util_get_MSB_mouse)(uint16_t val, uint8_t *msb) {
  if (msb == NULL) return 1;
  *msb = (uint8_t)((val >> 8) & 0xFF);
  return 0;
}

int (util_sys_in8b)(int port, uint8_t *value) {
  uint32_t res;
  if (sys_inb(port, &res) != 0) return 1;
  if (value != NULL) *value = (uint8_t)(res & 0xFF);
  return 0;
}

int (util_sys_in16b)(int port, uint16_t *value) {
  uint32_t res;
  if (sys_inb(port, &res) != 0) return 1;
  if (value != NULL) *value = (uint16_t)(res & 0xFFFF);
  return 0;
}

int (writeCmd)(uint8_t cmd) {
  if (!sys_outb(0x64, 0xD4)) return 1;
  if(!sys_outb(0x60, cmd)) return 1;
  return 0;
}

int (checkAckByte)() {
  uint8_t st;
  if (!util_sys_in8b(0x64, &st)) return 1;
  return !(st == 0xFA);
}
