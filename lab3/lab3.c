#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include "kbd.h"
#include <stdbool.h>
#include <stdint.h>

int main(int argc, char *argv[]) {
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

int(kbd_test_scan)() {
  uint8_t hook_id = 1;
  int r, ipc_status;
  message msg;
  if (kbd_subscribe_int(&hook_id) != 0) {
    printf("kbd_subscribe_int failed\n");
    return 1;
  }
  uint32_t irq_set = BIT(hook_id);
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

  return 0;
}

int(kbd_test_poll)() {
  /* To be completed by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int(kbd_test_timed_scan)(uint8_t n) {
  /* To be completed by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}
