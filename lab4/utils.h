#ifndef _LCOM_UTILS_H_
#define _LCOM_UTILS_H_

#include <lcom/lcf.h>

#include <stdint.h>

int (util_get_LSB)(uint16_t val, uint8_t *lsb);

int (util_get_MSB)(uint16_t val, uint8_t *msb);

int (util_sys_in8b)(int port, uint8_t *value);

int (util_sys_in16b)(int port, uint16_t *value);

int (writeCmd)(uint8_t cmd);

int (checkAckByte)(void);

#endif
