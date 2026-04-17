#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>

#include "kbc.h"

#include "../lab2/timer.h"


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
  
  int ipc_status,r;

  u8_t bit_no=1;
  kbcSubscribe(&bit_no);
  int irq_set=1<<bit_no;

message msg;

u8_t scancode=0;

while (kbcGetScancode()!=0x81) { /* You may want to use a different condition */
  /* Get a request message. */
  
  if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
    printf("driver_receive failed with: %d", r);
    continue;
  }
  if (is_ipc_notify(ipc_status)) { /* received notification */
    switch (_ENDPOINT_P(msg.m_source)) {
    case HARDWARE: /* hardware interrupt notification */
      if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */
        
        kbcIh();
        
        scancode=kbcGetScancode();

      
                                         /* process it */
      }
      break;
    default:
      break; /* no other notifications expected: do nothing */
    }
  } else { /* received a standard message, not a notification */
           /* no standard messages expected: do nothing */
  }
}
kbcUnsubscribe();

  return 0;
}

int(kbd_test_poll)() {

  u8_t scancode=0;
  while (kbcGetScancode()!=0x81) { /* You may want to use a different condition */
  /* Get a request message. */
  
  /* To be completed by the students */
    kbcIh();
    scancode=kbcGetScancode();
  }

  kbcEnableInterrupts();

  return 0;
}

int(kbd_test_timed_scan)(uint8_t n) {
  int ipc_status,r;

  u8_t kbc_bit_no=1;
  kbcSubscribe(&kbc_bit_no);
  int kbc_irq_set=1<<kbc_bit_no;

  u8_t timer_bit_no=1;
  timer_subscribe_int(&timer_bit_no);
  int timer_irq_set=1<<timer_bit_no;

message msg;

u8_t scancode=0;

int counter=0;

int f=60;

while (kbcGetScancode()!=0x81 & timer_get_counter()-counter<n*f) { /* You may want to use a different condition */
  /* Get a request message. */
  
  if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
    printf("driver_receive failed with: %d", r);
    continue;
  }
  if (is_ipc_notify(ipc_status)) { /* received notification */
    switch (_ENDPOINT_P(msg.m_source)) {
    case HARDWARE: /* hardware interrupt notification */
      if (msg.m_notify.interrupts & kbc_irq_set) { /* subscribed interrupt */
        
        kbcIh();
        
        scancode=kbcGetScancode();
        counter=timer_get_counter();
        /* process it */
      }
      if (msg.m_notify.interrupts & timer_irq_set) { /* subscribed interrupt */
        
        timer_int_handler();
       
        /* process it */
      }
      break;
    default:
      break; /* no other notifications expected: do nothing */
    }
  } else { /* received a standard message, not a notification */
           /* no standard messages expected: do nothing */
  }
}
kbcUnsubscribe();
timer_unsubscribe_int();


  return 0;
}


