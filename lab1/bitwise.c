#include "bitwise.h"
#include "stdarg.h"

#define TODO return 255

uint8_t clear(uint8_t msk, int pos) {
  uint8_t tmp = msk;
  uint8_t bit = (1 << pos);
  tmp &= ~bit;
  return tmp;
}

uint8_t set(uint8_t msk, int pos) {
  uint8_t tmp = msk;
  uint8_t bit = (1 << pos);
  tmp |= bit;
  return tmp;
}

bool is_set(uint8_t msk, int pos) {
  uint8_t tmp = msk;
  uint8_t bit = (1 << pos);
  return tmp & bit;
}

uint8_t lsb(uint16_t wide_msk) {
  uint16_t tmp = wide_msk;
  return (uint8_t)(tmp & 0xFF);
}

uint8_t msb(uint16_t wide_m1sk) {
  uint16_t tmp = wide_m1sk;
  return (uint8_t)(tmp >> 8);
}

uint8_t mask(int pos, ...) {
  uint8_t res = 0;
  va_list arg;
  va_start(arg, pos);
  int cur = pos;
  while (cur != MSK_END){
    uint8_t new = (1 << cur);
    res |= new;
    cur = va_arg(arg, int);
  }
  va_end(arg);
  return res;
}
