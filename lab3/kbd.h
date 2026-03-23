#ifndef KBD
#define KBD

#include <lcom/lcf.h>
#include "utilsKBD.h"

static uint8_t topByte = 0;
static uint8_t botByte = 0;
static uint8_t lastTopByte = 0;

int checkForErrorsInStatus(uint8_t* status, uint8_t* errors);
int readOutputBuffer(uint8_t* output);

int readCmdByte(uint8_t* cmd);
int writeCmdByte(uint8_t cmd);

int checkIfCanRead(int* res);
int checkIfCanWrite(int* res);

int enableIH();

void resetScanCode();
bool isTwoByteCode();

bool isValid();
uint8_t getSize();
bool isMake();
uint8_t getMSB();
uint8_t getLSB();
uint8_t getLastTopByte();

#endif
