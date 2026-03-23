//
// Created by greedisland on 3/15/26.
//

#ifndef GRUPO_2LEIC13_2_UTILS_H
#define GRUPO_2LEIC13_2_UTILS_H

#include <stdint.h>

int (util_get_LSB)(uint16_t val, uint8_t *lsb);

int(util_get_MSB)(uint16_t val, uint8_t *msb);

int (util_sys_inb)(int port, uint8_t *value);

#endif // GRUPO_2LEIC13_2_UTILS_H
