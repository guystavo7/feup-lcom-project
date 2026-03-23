//
// Created by greedisland on 3/15/26.
//

#ifndef GRUPO_2LEIC13_2_KEYBOARD_H
#define GRUPO_2LEIC13_2_KEYBOARD_H

#include <lcom/lcf.h>
#include <stdint.h>

void (kbc_ih)(void);

int (kbd_subscribe_int)(uint8_t *bit_no);

int (kbd_unsubscribe_int)(void);

uint8_t (get_scanCode)(void);

bool(get_readError)(void);

int (kbc_enable_interrupt)(void);

#endif // GRUPO_2LEIC13_2_KEYBOARD_H
