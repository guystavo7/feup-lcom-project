#ifndef KBD
#define KBD

#include <lcom/lcf.h>
#include "utilsKBD.h"

static uint16_t scanCode = 0;

int checkForErrorsInStatus(uint8_t* status, uint8_t* errors);
int readOutputBuffer(uint16_t* output);

int readCmdByte(uint8_t* cmd);
int writeCmdByte(uint8_t cmd);

int checkIfCanRead(int* res);
int checkIfCanWrite(int* res);

int enableIH();

bool isValid();
uint16_t getScanCode();
uint8_t getSize();
bool isMake();
uint8_t getMSB();
uint8_t getLSB();

#endif
