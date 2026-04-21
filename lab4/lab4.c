// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>
#include "utils.h"
#include "mouse.h"
#include "../lab2/timer.c"

// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


int (mouse_test_packet)(uint32_t cnt) {
  uint8_t hook_id = 12;
  int r, ipc_status;
  message msg;

  if (mouse_set_data_reporting_on() != 0) {
    printf("mouse_set_data_reporting_on failed\n");
    return 1;
  }

  if (mouse_subscribe_int(&hook_id) != 0) {
    printf("mouse_subscribe_int failed\n");
    mouse_set_data_reporting_off();
    return 1;
  }

  uint32_t irq_set = BIT(hook_id);
  while (cnt) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
      case HARDWARE: /* hardware interrupt notification */
        if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */
          mouse_ih();
          if (!isPacketReady()) {
            break;
          }
          struct packet p;
          p.bytes[0] = getPacket1();
          p.bytes[1] = getPacket2();
          p.bytes[2] = getPacket3();
          p.rb = isRightPressed();
          p.mb = isMiddlePressed();
          p.lb = isLeftPressed();
          p.delta_x = deltaX();
          p.delta_y = deltaY();
          p.x_ov = overflowX();
          p.y_ov = overflowY();
          cnt--;
          mouse_print_packet(&p);
          resetPacket();
        }
        break;
      default:
        break; /* no other notifications expected: do nothing */
      }
    } else { /* received a standard message, not a notification */
             /* no standard messages expected: do nothing */
    }
  }

  if (mouse_unsubscribe_int() != 0) {
    mouse_set_data_reporting_off();
    return 1;
  }

  if (mouse_set_data_reporting_off() != 0) {
    return 1;
  }

  return 0;
}

int (mouse_test_async)(uint8_t idle_time) {
  uint8_t hook_id = 12;
  uint8_t hook_id2 = 0;
  int r, ipc_status, ticks = idle_time * 60;
  message msg;

  if (timer_subscribe_int(&hook_id2)) return -1;

  if (mouse_subscribe_int(&hook_id) != 0) {
    printf("mouse_subscribe_int failed\n");
    mouse_set_data_reporting_off();
    return 1;
  }

  if (mouse_set_data_reporting_on() != 0) {
    printf("mouse_set_data_reporting_on failed\n");
    return 1;
  }

  uint32_t irq_set = BIT(hook_id);
  while (ticks) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
      case HARDWARE: /* hardware interrupt notification */
        if (msg.m_notify.interrupts & (1 << hook_id2)) {
          ticks--;
          if (ticks % 60 == 0) {
            timer_print_elapsed_time();
            timer_int_handler();
          }
        }
        if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */
          mouse_ih();
          if (!isPacketReady()) {
            break;
          }
          struct packet p;
          p.bytes[0] = getPacket1();
          p.bytes[1] = getPacket2();
          p.bytes[2] = getPacket3();
          p.rb = isRightPressed();
          p.mb = isMiddlePressed();
          p.lb = isLeftPressed();
          p.delta_x = deltaX();
          p.delta_y = deltaY();
          p.x_ov = overflowX();
          p.y_ov = overflowY();
          ticks = idle_time * 60;
          mouse_print_packet(&p);
          resetPacket();
        }
        break;
      default:
        break; /* no other notifications expected: do nothing */
      }
    } else { /* received a standard message, not a notification */
             /* no standard messages expected: do nothing */
    }
  }

  if (mouse_unsubscribe_int() != 0) {
    mouse_set_data_reporting_off();
    return 1;
  }

  if (mouse_set_data_reporting_off() != 0) {
    return 1;
  }

  return 0;
}
