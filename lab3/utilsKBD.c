#include "utilsKBD.h"

int sys_inb_8b(uint32_t addr, uint8_t* output) {
    uint32_t res;
    if (sys_inb(addr, &res)) return -1;

    *output = (uint8_t)res;
    return 0;
}

int kbdSubscribe(int* hook_id) {
    if (sys_irqsetpolicy(KBC_IRQ_LINE, IRQ_REENABLE | IRQ_EXCLUSIVE, hook_id)) return -1;
    return 0;
}

int kbdUnsubscribe(int* hook_id) {
    if (sys_irqrmpolicy(hook_id)) return -1;
    return 0;
}
