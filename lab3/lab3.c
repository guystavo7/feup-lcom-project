#include <lcom/lab3.h>
#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>

#include "kbd.h"

int main(int argc, char* argv[]) {
    // sets the language of LCF messages (can be either EN-US or PT-PT)
    lcf_set_language("EN-US");

    // enables to log function invocations that are being "wrapped" by LCF
    // [comment this out if you don't want/need it]
    lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

    // enables to save the output of printf function calls on a file
    // [comment this out if you don't want/need it]
    lcf_log_output("/home/lcom/labs/lab3/output.txt");

    // handles control over to LCF
    // [LCF handles command line arguments and invokes the right function]
    if (lcf_start(argc, argv))
        return 1;

    // LCF clean up tasks
    // [must be the last statement before return]
    lcf_cleanup();

    return 0;
}

void readKeys() {
    kbc_ih();
    if (!isValid()) return;

    bool make = isMake();
    uint8_t size = getSize();
    uint8_t bytes;
    bytes = getScanCode();
    kbd_print_scancode(make, size, &bytes);
}

int(kbd_test_scan)() {
    uint8_t bit_no = 1;
    int hook_id;
    hook_id = bit_no;

    kbdSubscribe(&hook_id);
    message msg;
    int r, ipc_status;

    while (getScanCode() != 0x81) {
        if ((r = driver_receive(ANY, &msg, &ipc_status))) {
            printf("Cannot read from %d driver\n", r);
            continue;
        }

        if (!is_ipc_notify(ipc_status)) continue;
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE:
                if (msg.m_notify.interrupts & (1 << bit_no)) readKeys();
                break;

            default:
                break;
        }
    }

    kbdUnsubscribe(&hook_id);
    return 0;
}

int(kbd_test_poll)() {
    while (getScanCode() != 0x81) {
        readKeys();
    }

    return 0;
}

int(kbd_test_timed_scan)(uint8_t n) {
    /* To be completed by the students */
    printf("%s is not yet implemented!\n", __func__);

    return 1;
}
