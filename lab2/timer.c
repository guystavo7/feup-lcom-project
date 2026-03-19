#include "timer.h"

#include "i8254.h"

#define CONTROL_PORT_ADDR 0x43
#define TIMER_BASE_ADDR 0x40

#define READ_BACK_COMMAND 0xC0
#define READ_BACK_COUNT_BIT 1 << 5
#define READ_BACK_STATUS_BIT 1 << 4
#define INITIALIZATION_MODE_MSK 0x30
#define COUNTING_MODE_MSK 0x0E
#define COUNTING_BASE_MSK 0x01
#define LSB_AND_MSB_MSK 0x30

int(timer_set_frequency)(uint8_t timer, uint32_t freq) {
    uint8_t st;
    if (timer_get_conf(timer, &st)) return -1;
    st = (st & 0x0F) | (timer << 6) | LSB_AND_MSB_MSK;

    if (sys_outb(CONTROL_PORT_ADDR, st)) return -1;
    uint16_t divider = TIMER_FREQ / freq;
    uint8_t value;

    util_get_LSB(divider, &value);
    if (sys_outb(TIMER_BASE_ADDR + timer, value)) return -1;

    util_get_MSB(divider, &value);
    if (sys_outb(TIMER_BASE_ADDR + timer, value)) return -1;

    return 0;
}

int(timer_subscribe_int)(uint8_t* bit_no) {
    hook_id = *bit_no;
    if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id)) return -1;
    return 0;
}

int(timer_unsubscribe_int)() {
    if (sys_irqrmpolicy(&hook_id)) return -1;
    return 0;
}

void(timer_int_handler)() {
    counter++;
}

int(timer_get_conf)(uint8_t timer, uint8_t* st) {
    uint32_t cmd = READ_BACK_COMMAND | READ_BACK_COUNT_BIT | (1 << (1 + timer));
    *st = 0;

    if (sys_outb(CONTROL_PORT_ADDR, cmd)) return -1;
    if (util_sys_inb(TIMER_BASE_ADDR + timer, st)) return -1;

    return 0;
}

uint8_t getMode(uint8_t mode) {
    mode = (mode & COUNTING_MODE_MSK) >> 1;
    return mode > 5 ? mode - 4 : mode;
}

int(timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
    union timer_status_field_val conf;

    switch (field) {
        case tsf_all:
            conf.byte = st;
            break;
        case tsf_initial:
            conf.in_mode = (st & INITIALIZATION_MODE_MSK) >> 4;
            break;
        case tsf_mode:
            conf.count_mode = getMode(st);
            break;
        case tsf_base:
            conf.bcd = st & 1;
            break;
    }

    if (timer_print_config(timer, field, conf)) return -1;
    return 0;
}

int getCounter() {
	return counter;
}

void resetCounter() {
	counter = 0;
}
