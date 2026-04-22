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
  if (timer > 2 || freq < 19 || freq > TIMER_FREQ) return 1;

  uint8_t st;
  if (timer_get_conf(timer, &st) != 0) return 1;
  uint8_t myCmd = (timer << 6) | TIMER_LSB_MSB | (st & 0x0F);
  if (sys_outb(TIMER_CTRL, myCmd) != 0) return 1;
  uint16_t divisor = TIMER_FREQ / freq;
  uint8_t lsb, msb;
  if (util_get_LSB(divisor, &lsb) != 0) return 1;
  if (util_get_MSB(divisor, &msb) != 0) return 1;
  int porta = TIMER_0 + timer;
  if (sys_outb(porta, lsb) != 0) return 1;
  if (sys_outb(porta, msb) != 0) return 1;
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
  if (timer > 2 || st == NULL) return 1;
  uint8_t myCmd = TIMER_RB_CMD | TIMER_RB_COUNT_;
  myCmd |= (1 << (timer + 1)); /*pq o bit 0 é reservado*/
  if (sys_outb(TIMER_CTRL, myCmd) != 0) return 1;
  int porta = TIMER_0 + timer;
  return util_sys_inb(porta, st);
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  /* To be implemented by the students */
  if (timer > 2) return 1;
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
    uint8_t mode = (st & 0x0E) >> 1;
    if (mode > 5) mode &= 0x03;
    conf.count_mode = mode;
  }
  
  return timer_print_config(timer, field, conf);
}
