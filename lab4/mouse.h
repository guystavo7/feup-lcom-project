#ifndef _LCOM_MOUSE_H_
#define _LCOM_MOUSE_H_

#include <lcom/lcf.h>
#include <stdint.h>
#include "i8254.h"
#include "utils.h"

uint8_t getPacket1(void);

uint8_t getPacket2(void);

uint8_t getPacket3(void);

int getCurrentHookId(void);

int (mouse_subscribe_int)(uint8_t *bit_no);

int (mouse_unsubscribe_int)(void);

int (mouse_set_data_reporting_on)(void);

int (mouse_set_data_reporting_off)(void);

void (mouse_ih)(void);

void (resetPacket)(void);

bool (isMiddlePressed)(void);

bool (isRightPressed)(void);

bool (isLeftPressed)(void);

int16_t (deltaX)(void);

int16_t (deltaY)(void);

bool (overflowX)(void);

bool (overflowY)(void);

bool (isPacketReady)(void);

#endif
