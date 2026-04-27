#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include "../lab3/kbd.c"
#include "spriteXMP.h"
#include "terminal.h"

#include <stdint.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    // sets the language of LCF messages (can be either EN-US or PT-PT)
    lcf_set_language("EN-US");

    // enables to log function invocations that are being "wrapped" by LCF
    // [comment this out if you don't want/need it]
    lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

    // enables to save the output of printf function calls on a file
    // [comment this out if you don't want/need it]
    lcf_log_output("/home/lcom/labs/lab5/output.txt");

    // handles control over to LCF
    // [LCF handles command line arguments and invokes the right function]
    if (lcf_start(argc, argv))
        return 1;

    // LCF clean up tasks
    // [must be the last statement before return]
    lcf_cleanup();

    return 0;
}

int drawRectangleTest(uint16_t mode, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
    Terminal* terminal;
    if ((terminal = newTerminal(mode)) == NULL) {
        printf("Error on starting new terminal\n");
        return -1;
    }

    for (int i = y; i < y + height; i++) drawLine(terminal, x, i, width, color);
    freeTerminal(terminal);
    return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {
    enterMode(mode);
    tickdelay(micros_to_ticks(delay * 1000000));
    vg_exit();
    return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
    if (drawRectangleTest(mode, x, y, width, height, color)) {
        printf("Cannot draw rectangle\n");
        vg_exit();
        return -1;
    }

    uint8_t bit_no = 1;
    int hook_id = 1;

    kbdSubscribe(&hook_id);
    message msg;
    int r, ipc_status;

    printf("Finished drawing\n");

    while (getLSB() != 0x81) {
        if ((r = driver_receive(ANY, &msg, &ipc_status))) {
            printf("Cannot read driver %x\n", r);
            continue;
        }

        if (!is_ipc_notify(ipc_status)) continue;
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE:
                if (msg.m_notify.interrupts & (1 << bit_no)) {
                    kbc_ih();
                }

                break;
            default:
                break;
        }
    }

    vg_exit();
    kbdUnsubscribe(&hook_id);
    return 0;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
	Terminal* terminal = newTerminal(0x105);
	enum xpm_image_type type = XPM_INDEXED;
	SpriteXPM* sprite = newSpriteXPM(xpm, type);

	drawSpriteXPM(sprite, terminal, x, y);

    uint8_t bit_no = 1;
    int hook_id = 1;

    kbdSubscribe(&hook_id);
    message msg;
    int r, ipc_status;

    while (getLSB() != 0x81) {
        if ((r = driver_receive(ANY, &msg, &ipc_status))) {
            printf("Cannot read driver %x\n", r);
            continue;
        }

        if (!is_ipc_notify(ipc_status)) continue;
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE:
                if (msg.m_notify.interrupts & (1 << bit_no)) {
                    kbc_ih();
                }

                break;
            default:
                break;
        }
    }

    vg_exit();
    kbdUnsubscribe(&hook_id);
 
	freeSpriteXPM(sprite);
	freeTerminal(terminal);
    return 0;
}
