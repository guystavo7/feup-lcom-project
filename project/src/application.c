#include "../../lab2/timer.h"
#include "../../lab3/kbc.h"
#include "../../lab4/mouse.h"
#include "../../lab5/terminal.h"

#include "../include/game.h"
#include "../include/utils/cursorHandler.h"
#include "../include/utils/inputHandler.h"
#include "../include/utils/utils.h"

#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

/** If proj is started from project/src or project/build, cwd is not project/; chdir once so resource paths need no "..". */
static void ensure_project_resource_cwd(void) {
    if (access("resources/map/grid_walls.csv", R_OK) == 0) return;
    (void)chdir("..");
}

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
    ensure_project_resource_cwd();

    int hook_id_timer = TIMER_IRQ_LINE;
    int hook_id_kbd = KBC_IRQ_LINE;
    int hook_id_mouse = MOUSE_IRQ_LINE;

    mouseSubscribe(&hook_id_mouse);
    timer_subscribe((uint8_t*)&hook_id_timer);
    kbcSubscribe((uint8_t*)&hook_id_kbd);
    Terminal* terminal = newTerminal(0x11B);
    startGame(terminal);

    enableDataReport();
    int lastFrameTime = timer_get_counter();

    int r, ipc_status;
    message msg;
	bool isRunning = true;
 
    while (isRunning) { 
        if ((r = driver_receive(ANY, &msg, &ipc_status))) {
            printf("Driver receive failed: %d\n", r);
            continue;
        }

        if (!is_ipc_notify(ipc_status)) continue;
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE:
                if (msg.m_notify.interrupts & (1 << TIMER_IRQ_LINE)) {
					if (getKeyPressed(getInputHandler(), KEYCODE_ENTER)) isRunning = false;

                    timer_int_handler();
                    refreshTerminal(terminal);
                    int currFrameTime = timer_get_counter();
                    float deltaTime = (float)(currFrameTime - lastFrameTime) / 60;

                    if (!gameLoop(terminal, deltaTime)) isRunning = false;
                    lastFrameTime = currFrameTime;
                }

                if (msg.m_notify.interrupts & (1 << MOUSE_IRQ_LINE)) {
                    mouse_ih();
                    if (isReady()) {
                        if (!(isOverflowingX() || isOverflowingY())) {
                            cursorUpdateData(getCursor(), getDeltaX(), -getDeltaY(), pressedLeftButton(), pressedRightButton(), pressedMiddleButton());
                        }

                        cleanBytes();
                    }
                }

                if (msg.m_notify.interrupts & (1 << KBC_IRQ_LINE)) {
                    kbcIh();
                    if (kbcIsKeyReady()) {
                        if (!kbcIsTwoByteCode()) {
                            uint16_t keycode = (uint16_t)(kbcGetScancode() & ~(0x80));
                            readKey(getInputHandler(), keycode, kbcIsMake());
                        }

                        kbcResetKeys();
                    }
                }

                break;
            default:
                break;
        }
    }

    disableDataReport();
    mouseUnsubscribe(&hook_id_mouse);
    timer_unsubscribe_int();
    kbcUnsubscribe();
    cursorFree(getCursor());

    exitGame();
    freeTerminal(terminal);
    return 0;
}
