#include <lcom/lcf.h>
#include "kbc.h"

#define DELAY_US    20000

static int hook_id=1;
static u8_t g_scancode=0;


int (kbcSubscribe)(uint8_t *bit_no) {
    /* To be implemented by the students */
    
    
  *bit_no=hook_id;
  int ret=sys_irqsetpolicy( KBC1_IRQ,IRQ_REENABLE | IRQ_EXCLUSIVE,&hook_id);
  


  return ret;
}

int (kbcUnsubscribe)() {
  /* To be implemented by the students */
  
  
  return sys_irqrmpolicy(&hook_id);
}



void (kbcIh)() {
    // to be completed

    u8_t status=0,scancode=0;


    bool make;
    uint8_t size,bytes[2];


    util_sys_inb(KBC_ST_REG,&status);

    bool erro=((status&0x80)>>7) |((status&0x40)>>6);

    
    
    if(  (erro == 0) && (status&0x1) ){  /*&& (status&0x1)*/

      

      util_sys_inb(KBC_IO_BUF_REG,&scancode);
      bytes[0]=scancode;
      make= ((scancode&0x80)==0);
      size=1;

      if(scancode==0xE0){
        util_sys_inb(KBC_IO_BUF_REG,&scancode);
        bytes[1]=scancode;
        make= ((scancode&0x80)==0);
        size=2;
      }


      g_scancode=scancode;
      kbd_print_scancode( make,  size,  bytes); 
    }
    

}

u8_t kbcGetScancode(){
  return g_scancode;
}


int kbcSendCommand(u8_t cmd){
  u8_t stat=0;
  int counter=10;
while ( counter-- ) {
    util_sys_inb(KBC_ST_REG, &stat); /* assuming it returns OK */
    /* loop while 8042 input buffer is not empty */
    if( (stat & KBC_ST_IBF) == 0 ) {
        sys_outb(KBC_CMD_REG, cmd); /* no args command */
        return 0;
    }
    tickdelay(micros_to_ticks(DELAY_US));
}

  return -1;

// tickdelay(micros_to_ticks(DELAY_US));
}

u8_t kbcReadCommandResponse(){
  u8_t stat,data;

  int counter=10;
while ( counter-- ) {
    util_sys_inb(KBC_ST_REG, &stat); /* assuming it returns OK */
    /* loop while 8042 output buffer is empty */
    if( stat & KBC_OBF ) {
        util_sys_inb(KBC_OUT_BUF, &data); /* ass. it returns OK */
        if ( (stat &(KBC_PARITY_ERR | KBC_TIMEOUT_ERR)) == 0 )
            return data;
        else
            return -1;
    }
    tickdelay(micros_to_ticks(DELAY_US));
}

  return -1;

}

u8_t kbcEnableInterface(){
  return kbcSendCommand(0xAE);
}

u8_t kbcDisableInterface(){
  return kbcSendCommand(0xAD);
}

bool kbc_self_check(){
  int ret;
  ret=kbcSendCommand(0xAA);
  u8_t data=kbcReadCommandResponse();
  return data==0x55;
}

bool kbc_check_interface(){
  int ret;
  ret=kbcSendCommand(0xAB);
  u8_t data=kbcReadCommandResponse();
  return data==0;
}

u8_t kbc_read_command_byte(){
  kbcSendCommand(0x20);
  return kbcReadCommandResponse();
}

u8_t kbcWriteCommandByte(u8_t cmd_byte){
  int ret=kbcSendCommand(0x60);
  sys_outb(KBC_IO_BUF_REG,cmd_byte);
  return ret;
}

void kbcEnableInterrupts(){
  u8_t cmd_byte= kbc_read_command_byte();
  cmd_byte= cmd_byte& (~0x10);
  cmd_byte=cmd_byte | (0x01);
  kbcWriteCommandByte(cmd_byte);
  
}

void kbcDisableInterrupts(){
  u8_t cmd_byte= kbc_read_command_byte();
  cmd_byte= cmd_byte| (0x10);
  cmd_byte=cmd_byte & (~0x01);
  kbcWriteCommandByte(cmd_byte);
  
}

