// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>
#include "graphics.h"
#include "i8254.h"
#include "../lab3/kbd.h"

int setGraphicsMode(uint16_t mode);
int setTextMode(void);

// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
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

int(video_test_init)(uint16_t mode, uint8_t delay) {
  if (setGraphicsMode(mode) != 0) return 1;

  tickdelay(micros_to_ticks((uint32_t) delay * 1000000));

  if (vg_exit() != 0) return 1;

  return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  if(vg_init(mode) == NULL) return 1;
  
  uint8_t hook_id = 1;
  int r, ipc_status;
  message msg;
  if (kbd_subscribe_int(&hook_id) != 0) {
    printf("kbd_subscribe_int failed\n");
    return 1;
  }
  uint32_t irq_set = BIT(hook_id);

  vg_draw_rect(x, y, width, height, color);

  while (getCurrentScancode() != KBD_ESC) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
      case HARDWARE: /* hardware interrupt notification */
        if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */
          kbc_ih();
          uint8_t bytes[2];
          int size;
          if (getCurrentScancode() > 0xFF) {
            size = 2;
            bytes[0] = (uint8_t)((getCurrentScancode() >> 8) & 0xFF);
            bytes[1] = (uint8_t)(getCurrentScancode() & 0xFF);
          }
          else {
            size = 1;
            bytes[0] = (uint8_t)(getCurrentScancode() & 0xFF);
          }
          bool is_make = !(bytes[size - 1] & 0x80);
          kbd_print_scancode(is_make, size, bytes);
        }
        break;
      default:
        break; /* no other notifications expected: do nothing */
      }
    } else { /* received a standard message, not a notification */
             /* no standard messages expected: do nothing */
    }
  }
  kbd_unsubscribe_int();
  if (vg_exit() != 0) return 1;
  return 0;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {

    return 1;
}
