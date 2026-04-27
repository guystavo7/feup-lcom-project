#include "../../lab2/timer.h"
#include "../../lab3/kbc.h"
#include "../../lab4/mouse.h"
#include "../../lab5/spriteXMP.h"
#include "../../lab5/terminal.h"

#include "../include/game.h"
#include "../include/utils.h"

#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you

int main(int argc, char* argv[]) {
    // sets the language of LCF messages (can be either EN-US or PT-PT)
    lcf_set_language("EN-US");

    // enables to log function invocations that are being "wrapped" by LCF
    // [comment this out if you don't want/need/ it]
    lcf_trace_calls("/home/lcom/labs/project/trace.txt");

    // enables to save the output of printf function calls on a file
    // [comment this out if you don't want/need it]
    lcf_log_output("/home/lcom/labs/project/output.txt");

    // handles control over to LCF
    // [LCF handles command line arguments and invokes the right function]
    if (lcf_start(argc, argv)) return 1;

    // LCF clean up tasks
    // [must be the last statement before return]
    lcf_cleanup();

    return 0;
}

int(proj_main_loop)(int argc, char* argv[]) {
    int hook_id_timer = TIMER_IRQ_LINE;
    int hook_id_kbd = KBC_IRQ_LINE;
    int hook_id_mouse = MOUSE_IRQ_LINE;

    mouseSubscribe(&hook_id_mouse);
    timer_subscribe_int((uint8_t*)&hook_id_timer);
    kbcSubscribe((uint8_t*)&hook_id_kbd);

    enableDataReport();

    int r, ipc_status;
    message msg;

    while (timer_get_counter() <= 10 * 60) {
        if ((r = driver_receive(ANY, &msg, &ipc_status))) {
            printf("Driver receive failed: %d\n", r);
            continue;
        }

        if (!is_ipc_notify(ipc_status)) continue;
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE:
                if (msg.m_notify.interrupts & (1 << TIMER_IRQ_LINE)) {
                    timer_int_handler();
                }

                if (msg.m_notify.interrupts & (1 << MOUSE_IRQ_LINE)) {
                    printf("Used mouse\n");
                }

                if (msg.m_notify.interrupts & (1 << KBC_IRQ_LINE)) {
					kbcIh();
					if (isKeyReady()) resetKeys();
                }

                break;
            default:
                break;
        }
    }

    mouseUnsubscribe(&hook_id_mouse);
    timer_unsubscribe_int();
    kbcUnsubscribe();
    disableDataReport();
    return 0;
}
