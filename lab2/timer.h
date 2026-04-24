#ifndef TIMER
#define TIMER

#include <lcom/lcf.h>

#define CONTROL_PORT_ADDR 0x43
#define TIMER_BASE_ADDR 0x40

#define READ_BACK_COMMAND 0xC0
#define READ_BACK_COUNT_BIT 1 << 5
#define READ_BACK_STATUS_BIT 1 << 4
#define INITIALIZATION_MODE_MSK 0x30
#define COUNTING_MODE_MSK 0x0E
#define COUNTING_BASE_MSK 0x01
#define LSB_AND_MSB_MSK 0x30

static int counter = 0;
static int hook_id;

int(timer_set_frequency)(uint8_t timer, uint32_t freq);

int(timer_subscribe_int)(uint8_t* bit_no);
int(timer_unsubscribe_int)();
int (timerSubscribe)(int* hook_id);

void(timer_int_handler)();

int(timer_get_conf)(uint8_t timer, uint8_t* st);
uint8_t getMode(uint8_t mode);
int(timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field);

int getCounter();
void resetCounter();

#endif
