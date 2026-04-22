//
// Created by greedisland on 3/15/26.
//

#ifndef GRUPO_2LEIC13_2_KEYBOARD_H
#define GRUPO_2LEIC13_2_KEYBOARD_H

#include <lcom/lcf.h>
#include <stdint.h>


void (mouse_ih)(void);

int (mouse_subscribe_int)(uint8_t *bit_no);

int (mouse_unsubscribe_int)(void);

int mouse_get_myCounter();

struct packet mouse_get_pck();

int mouse_get_pck_counter();


void mouse_enable_data_reporting_d();

void mouse_disable_data_reporting();


int (mouse_enable_interrupt)(void);

#endif // GRUPO_2LEIC13_2_KEYBOARD_H
