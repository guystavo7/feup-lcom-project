#include <lcom/lcf.h>
#include <stdint.h>

#define KBC1_IRQ 1

#define KBC_ST_REG 0x64
#define KBC_CMD_REG 0x64
#define KBC_ST_IBF 0x60

#define KBC_IO_BUF_REG 0x60

#define KBC_OUT_BUF 0x60
#define KBC_PARITY_ERR BIT(7)
#define KBC_TIMEOUT_ERR BIT(6)
#define KBC_OBF BIT(0)

int kbcSubscribe(uint8_t *bit_no);
int kbcUnsubscribe();

uint16_t kbcGetScancode();
uint16_t getLastByte();

int kbcSendCommand(uint8_t cmd);
uint8_t kbcReadCommandResponse();

void kbcEnableInterrupts();

void(kbcIh)();
void resetKeys();

uint8_t getMSB();
uint8_t getLSB();

bool isTwoByteCode();
uint8_t getLastTopByte();
uint8_t getSize();
bool isMake();
bool isKeyReady();
