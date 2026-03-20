#ifndef _LCOM_KBD_H_
#define _LCOM_KBD_H_

#include <lcom/lcf.h>
#include <stdint.h>
#include "i8254.h"
#include "utils.h"

extern int hookId;
extern uint16_t scancode;

uint16_t getCurrentScancode(void);
int getCurrentHookId(void);

int (kbd_subscribe_int)(uint8_t *bit_no);
int (kbd_unsubscribe_int)(void);
void (kbc_ih)(void);

#endif
