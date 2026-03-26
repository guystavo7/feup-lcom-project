#include "kbd.h"

int checkForErrorsInStatus(uint8_t* status, uint8_t* errors) {
    // Read status and check for errors (errors == 0 OK)
    if (sys_inb_8b(STATUS_COMMAND_ADDR, status)) return -1;
    *errors = *status & (PARITY_MSK | TIMEOUT_MSK);
    return 0;
}

int checkIfCanRead(int* res) {
    // Read status and check for errors and if output buffer is full (res == 1 OK to read)
    uint8_t status, errors;
    if (checkForErrorsInStatus(&status, &errors)) return -1;
    if (!errors) *res = status & OBF_MSK;
    return 0;
}

int checkIfCanWrite(int* res) {
    // Read status and check for errors and if input is full (res == 1 OK to read)
    uint8_t status, errors;
    if (checkForErrorsInStatus(&status, &errors) || errors) return -1;
    if (!errors) *res = status & IBF_MSK;

    // Invert the value -> if res == 1 cannot to write so res = 0
	if (*res) {
		*res = 0;
	} else {
		*res = 1;
	}

    return 0;
}

int readOutputBuffer(uint8_t* output) {
    // Read output buffer (needs to check if can read)
    int res = 0;
    if (checkIfCanRead(&res)) return -1;
    if (res && sys_inb_8b(INPUT_OUTPUT_ADDR, output)) return -1;
    return 0;
}

int readCmdByte(uint8_t* cmd) {
    if (sys_outb(STATUS_COMMAND_ADDR, READ_CMD)) return -1;
    int res = 0;

    for (int i = 0; i < TRIES; i++) {
        if (!checkIfCanRead(&res) && res && !sys_inb_8b(INPUT_OUTPUT_ADDR, cmd)) return 0;
        tickdelay(micros_to_ticks(DELAY));
    }

    return -1;
}

int writeCmdByte(uint8_t cmd) {
    if (sys_outb(STATUS_COMMAND_ADDR, WRITE_CMD)) return -1;
    int res = 0;

    for (int i = 0; i < TRIES; i++) {
		int res1 = checkIfCanWrite(&res);
        if (!checkIfCanWrite(&res) && res && !sys_out_8b(INPUT_OUTPUT_ADDR, cmd)) return 0;
        tickdelay(micros_to_ticks(DELAY));
    }

    return -1;
}

int enableIH() {
    uint8_t cmd;
    if (readCmdByte(&cmd)) return -1;
    cmd |= KBD_IH_ENABLE_MSK;
    if (writeCmdByte(cmd)) return -1;
	readCmdByte(&cmd);

    return 0;
}

void(kbc_ih)() {
    uint8_t output = 0;
    if (readOutputBuffer(&output)) return;
    if (topByte == 0) {
        topByte = output;
    } else {
        botByte = output;
    }
}

void resetScanCode() {
    lastTopByte = topByte;
    topByte = 0;
    botByte = 0;
}

bool isTwoByteCode() {
    return topByte == 0xe0;
}

bool isValid() {
    // If scanCode is invalid or an error occurr, the scancode is 0
    return topByte;
}

uint8_t getLastTopByte() {
    return lastTopByte;
}

uint8_t getMSB() {
    return topByte;
}
uint8_t getLSB() {
    return botByte;
}

uint8_t getSize() {
    return getLSB() ? 2 : 1;
}

bool isMake() {
    uint8_t byte = botByte;
    if (!botByte) byte = topByte;
    return !(byte & MAKE_MSK);
}
