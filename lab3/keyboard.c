//
// Created by greedisland on 3/15/26.
//

#include "keyboard.h"
#include <lcom/lcf.h>
#include "i8254.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "utils.h"

#define DELAY_US 20000
#define KBC_ST_REG 0x64
#define KBC_CMD_REG 0x64
#define KBC_ST_IBF 0x60
#define KBC_OUT_BUF 0x60
#define KBC_PARITY_ERR BIT(7)
#define KBC_TIMEOUT_ERR BIT(6)
#define KBC_OBF BIT(0)

static uint8_t scanCode = 0;
static bool readError = false;
static int hook_id = 0; /*identificar o driver, podia comecar com qualquer valor*/

void(kbc_ih)(void)
{
    uint8_t status;
    uint8_t data;
    if (util_sys_inb(0x64, &status) != 0)
    {
        
        return; // para verificar se esta disponivel para ler
    }

    readError=!((status & BIT(0)) == 1);
    if ((status & BIT(0)) == 1)
    {
        util_sys_inb(0x60, &data);
        if ((status & (BIT(6) | BIT(7))) == 0)
        {
            readError = false;
            scanCode = data;
        }
        else
            readError = true;
    }
}

int(kbd_subscribe_int)(uint8_t *bit_no)
{
    if (bit_no == NULL)
        return 1;
    *bit_no = (uint8_t)hook_id;
    return sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id); /*pedir para avisar sempre que o keyboard_IRQ tiver uma interrupcao*/
    /*O Minix já tem o seu próprio programa (driver) a correr em fundo que lê o teclado (é por isso que consegues escrever no terminal do Minix).
    Se não usares o IRQ_EXCLUSIVE, o Minix e o teu programa vão "lutar" pelas teclas*/
}

int(kbd_unsubscribe_int)(void)
{
    return sys_irqrmpolicy(&hook_id);
}

uint8_t(get_scanCode)()
{
    return scanCode;
}

bool(get_readError)()
{
    return readError;
}

int kbc_read_response()
{
    uint8_t stat, data;
    int counter = 20;
    while (counter--)
    {
        util_sys_inb(KBC_ST_REG, &stat); /* assuming it returns OK */
        /* loop while 8042 output buffer is empty */
        if (stat & KBC_OBF)
        {
            util_sys_inb(KBC_OUT_BUF, &data); /* ass. it returns OK */
            if ((stat & (KBC_PARITY_ERR | KBC_TIMEOUT_ERR)) == 0)
                return data;
            else
                return -1;
        }
        tickdelay(micros_to_ticks(DELAY_US));
    }

    return 1;
}

int kbc_send_command(int cmd)
{
    uint8_t stat;
    int counter = 20;
    while (counter--)
    {
        util_sys_inb(KBC_ST_REG, &stat); /* assuming it returns OK */
        /* loop while 8042 input buffer is not empty */
        if ((stat & KBC_ST_IBF) == 0)
        {
            sys_outb(KBC_CMD_REG, cmd); /* no args command */
            return 0;
        }
        tickdelay(micros_to_ticks(DELAY_US));
    }

    return 1;
}

int kbc_read_command_byte()
{
    kbc_send_command(0x20);
    return kbc_read_response();
}

int kbc_write_command_byte(uint8_t cmdByte)
{
    kbc_send_command(0x60);
    return sys_outb(KBC_OUT_BUF, cmdByte);
}

int(kbc_enable_interrupt)()
{
    uint8_t cmdByte = kbc_read_command_byte();
    cmdByte |= 1;
    return kbc_write_command_byte(cmdByte);
}
