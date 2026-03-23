#include <lcom/lcf.h>
#include <lcom/timer.h>
#include <stdint.h>
#include "i8254.h"
#include "timer.h"
#include "utils.h"

static int hook_id = 1; /*identificar o driver, podia comecar com qualquer valor*/
static uint32_t count = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  /* To be implemented by the students */
  uint8_t st;
  timer_get_conf(timer, &st);
  uint8_t myCmd = (timer << 6) | (1 << 5) | (1 << 4) | (st & 0x0F);
  sys_outb(0x43, myCmd);
  uint16_t divisor = TIMER_FREQ / freq;
  uint8_t lsb, msb;
  util_get_LSB(divisor, &lsb);
  util_get_MSB(divisor, &msb);
  int porta = 0x40 + timer;
  sys_outb(porta, lsb);
  sys_outb(porta, msb);
  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
    /* To be implemented by the students */
  if (bit_no == NULL) return 1;
  count = 0;
  *bit_no = (uint8_t) hook_id;
  return sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id); /*pedir para avisar sempre que o IRQ0 tiver uma interrupcao*/
}

int (timer_unsubscribe_int)() {
  /* To be implemented by the students */
  return sys_irqrmpolicy(&hook_id);
}

void (timer_int_handler)() {
  /* To be implemented by the students */
  count++;
}

uint32_t (get_count)(){
  return count;
}




int timer_get_conf(uint8_t timer, uint8_t *st) {
  /* To be implemented by the students */
  uint8_t myCmd = (1 << 7) | (1 << 6) | (1 << 5);
  myCmd |= (1 << (timer + 1)); /*pq o bit 0 é reservado*/
  sys_outb(0x43, myCmd);
  int porta = 0x40 + timer;
  return util_sys_inb(porta, st);
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  /* To be implemented by the students */
  union timer_status_field_val conf;
  if(field == tsf_all){
    conf.byte = st;
  }
  else if(field == tsf_base){
    conf.bcd = st & 1;
  }
  else if(field == tsf_initial){
    uint8_t tranform = (st & 0x30) >> 4;
    conf.in_mode = (enum timer_init) tranform;
  }
  else if(field == tsf_mode){
    conf.count_mode = (st & 0x0E) >> 1;
  }
  
  return timer_print_config(timer, field, conf);
}
