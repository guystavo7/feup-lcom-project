//
// Created by greedisland on 3/15/26.
//

#ifndef GRUPO_2LEIC13_2_TIMER_H
#define GRUPO_2LEIC13_2_TIMER_H

#include <lcom/lcf.h>

int (timer_set_frequency)(uint8_t timer, uint32_t freq);

int (timer_subscribe_int)(uint8_t *bit_no);

int (timer_unsubscribe_int)();

void (timer_int_handler)();

uint32_t (get_count)();

int (timer_get_conf)(uint8_t timer, uint8_t *st);

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field);

#endif // GRUPO_2LEIC13_2_TIMER_H
