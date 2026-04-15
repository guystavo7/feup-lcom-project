#include <lcom/lcf.h>
#include <stdint.h>
#include "i8254.h"
#include "utils.h"
#include "mouse.h"

#define KBC_ST_REG 0x64
#define KBC_CMD_REG 0x64
#define KBC_OUT_BUF 0x60
#define KBC_IN_BUF 0x60
#define KBC_OBF BIT(0)
#define KBC_IBF BIT(1)
#define KBC_AUX BIT(5)

#define KBC_WRITE_BYTE_TO_MOUSE 0xD4

#define MOUSE_ACK 0xFA
#define MOUSE_NACK 0xFE
#define MOUSE_ERROR 0xFC

static int hookId = 1;
static uint8_t packet1 = 0;
static uint8_t packet2 = 0;
static uint8_t packet3 = 0;
static uint8_t packet_bytes[3] = {0, 0, 0};
static uint8_t byte_index = 0;
static bool packet_ready = false;

static int kbc_wait_input_empty(void) {
  uint8_t status;
  for (int attempts = 0; attempts < 20; attempts++) {
    if (util_sys_in8b(KBC_ST_REG, &status) != 0) return 1;
    if (!(status & KBC_IBF)) return 0;
    tickdelay(micros_to_ticks(20000));
  }
  return 1;
}

static int mouse_read_ack(uint8_t *ack) {
  uint8_t status;
  for (int attempts = 0; attempts < 20; attempts++) {
    if (util_sys_in8b(KBC_ST_REG, &status) != 0) return 1;
    if (status & (KBD_PARITY_ERROR | KBD_TIMEOUT_ERROR)) return 1;
    if ((status & KBC_OBF) && (status & KBC_AUX)) {
      if (util_sys_in8b(KBC_OUT_BUF, ack) != 0) return 1;
      return 0;
    }
    tickdelay(micros_to_ticks(20000));
  }
  return 1;
}

static int mouse_send_command(uint8_t cmd) {
  uint8_t ack;
  for (int tries = 0; tries < 3; tries++) {
    if (kbc_wait_input_empty() != 0) return 1;
    if (sys_outb(KBC_CMD_REG, KBC_WRITE_BYTE_TO_MOUSE) != 0) return 1;
    if (kbc_wait_input_empty() != 0) return 1;
    if (sys_outb(KBC_IN_BUF, cmd) != 0) return 1;
    if (mouse_read_ack(&ack) != 0) return 1;
    if (ack == MOUSE_ACK) return 0;
    if (ack == MOUSE_ERROR) return 1;
    if (ack != MOUSE_NACK) return 1;
  }
  return 1;
}

uint8_t getPacket1(void) {
  return packet1;
}

uint8_t getPacket2(void) {
  return packet2;
}
uint8_t getPacket3(void) {
  return packet3;
}

bool isMiddlePressed(void) {
  return packet1 & BIT(2);
}

bool isRightPressed(void) {
  return packet1 & BIT(1);
}

bool isLeftPressed(void) {
  return packet1 & BIT(0);
}

int16_t deltaX(void) {
  return (packet1 & BIT(4)) ? (int16_t)((int16_t)packet2 - 256) : (int16_t)packet2;
}

int16_t deltaY(void) {
  return (packet1 & BIT(5)) ? (int16_t)((int16_t)packet3 - 256) : (int16_t)packet3;
}

bool overflowX(void) {
  return packet1 & BIT(6);
}

bool overflowY(void) {
  return packet1 & BIT(7);
}

int getCurrentHookId(void) {
  return hookId;
}

int (mouse_subscribe_int)(uint8_t *bit_no) {
  hookId = *bit_no;
  *bit_no = (uint8_t) hookId;
  if (sys_irqsetpolicy(KBD_AUX_IRQ , IRQ_REENABLE | IRQ_EXCLUSIVE, &hookId) != 0) return 1;
  return 0;
}

int (mouse_unsubscribe_int)(void) {
  if (sys_irqrmpolicy(&hookId) != 0) return 1;
  return 0;
}

int (mouse_set_data_reporting_on)(void) {
  return mouse_send_command(MOUSE_SMODE);
}

int (mouse_set_data_reporting_off)(void) {
  return mouse_send_command(MOUSE_RMODE);
}

void (mouse_ih)(void) {
  uint8_t st, data;
  if (util_sys_in8b(KBC_ST_REG, &st) != 0) return;
  if (st & (KBD_PARITY_ERROR | KBD_TIMEOUT_ERROR)) return;
  if (!(st & KBC_OBF)) return;
  if (!(st & KBC_AUX)) return;
  if (util_sys_in8b(KBC_OUT_BUF, &data) != 0) return;

  if (byte_index == 0 && !(data & BIT(3))) {
    return;
  }

  packet_bytes[byte_index] = data;
  byte_index++;

  if (byte_index == 3) {
    packet1 = packet_bytes[0];
    packet2 = packet_bytes[1];
    packet3 = packet_bytes[2];
    packet_ready = true;
    byte_index = 0;
  }
}

void (resetPacket)(void) { 
  packet_ready = false;
}

bool (isPacketReady)(void) {
  return packet_ready;
}
