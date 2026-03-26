#ifndef UTILS_KBD
#define UTILS_KBD

#include <lcom/lcf.h> 

#define KBC_IRQ_LINE 0x01

#define INPUT_OUTPUT_ADDR 0x60
#define STATUS_COMMAND_ADDR 0x64

#define PARITY_MSK 0x80
#define TIMEOUT_MSK 0x40
#define OBF_MSK 0x01
#define IBF_MSK 0x02

#define MSB_MSK 0xFF00
#define LSB_MSK 0x00FF
#define MAKE_MSK 0x80

#define READ_CMD 0x20
#define WRITE_CMD 0x60
#define CHECK_KBC_CMD 0xAA
#define CHECK_KBD_CMD 0xAB
#define DISABLE_KBD_CMD 0xAD
#define ENABLE_KBD_CMD 0xAE

#define KBD_IH_ENABLE_MSK 0x01

#define TRIES 10
#define DELAY 20000

int sys_inb_8b(uint32_t addr, uint8_t* output);
int sys_out_8b(uint32_t addr, uint8_t input);

int kbdSubscribe(int* hook_id);
int kbdUnsubscribe(int* hook_id);

#endif
