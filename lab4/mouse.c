//
// Created by greedisland on 3/15/26.
//

#include "mouse.h"
#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <i8254.h>
#include <utils.h>

#define DELAY_US 20000
#define MOUSE_ST_REG 0x64
#define MOUSE_CMD_REG 0x64
#define MOUSE_ST_IBF BIT(1)
#define MOUSE_OUT_BUF 0x60
#define MOUSE_PARITY_ERR BIT(7)
#define MOUSE_TIMEOUT_ERR BIT(6)
#define MOUSE_OBF BIT(0)

#define MOUSE_IRQ 12
#define MOUSE_AUX_CMD 0xD4

static uint8_t scanCode = 0;
static struct packet pck;
static int pck_counter = 0;
static bool readError = false;
static int hook_id = MOUSE_IRQ; /*identificar o driver, podia comecar com qualquer valor*/

static int myCounter = 0;

int mouse_read_out_buf();
int mouse_send_command_no_arg();

void(mouse_ih)(void)
{
    int byte = mouse_read_out_buf();
    if ((byte & BIT(3)) != 1 && pck_counter == 0)
    {
        //  return;
    }

    pck.bytes[pck_counter] = byte;

    pck_counter++;

    if (pck_counter == 3)
    { // atualiza apenas quando chega o terceiro byte do pacote
        pck.lb = pck.bytes[0] & BIT(0);
        pck.rb = pck.bytes[0] & BIT(1);
        pck.mb = pck.bytes[0] & BIT(2);
        pck.x_ov = pck.bytes[0] & BIT(6);
        pck.y_ov = pck.bytes[0] & BIT(7);
        pck.delta_x = pck.bytes[1];
        pck.delta_y = pck.bytes[2];
        if (pck.bytes[0] & BIT(4))
        {
            pck.delta_x |= 0xFF00;
        }
        if (pck.bytes[0] & BIT(5))
        {
            pck.delta_y |= 0xFF00;
        }
        myCounter++;
    }

    pck_counter %= 3;
}

int mouse_get_myCounter()
{
    return myCounter;
}

struct packet mouse_get_pck()
{
    return pck;
}

int mouse_get_pck_counter()
{
    return pck_counter;
}

int(mouse_subscribe_int)(uint8_t *bit_no)
{
    if (bit_no == NULL)
        return 1;
    *bit_no = (uint8_t)hook_id;
    return sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id); /*pedir para avisar sempre que o keyboard_IRQ tiver uma interrupcao*/
    /*O Minix já tem o seu próprio programa (driver) a correr em fundo que lê o teclado (é por isso que consegues escrever no terminal do Minix).
    Se não usares o IRQ_EXCLUSIVE, o Minix e o teu programa vão "lutar" pelas teclas*/
}

int(mouse_unsubscribe_int)(void)
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

int mouse_read_out_buf()
{
    uint8_t stat, data;
    int counter = 20;
    while (counter--)
    {

        util_sys_inb(MOUSE_ST_REG, &stat); /* assuming it returns OK */
        /* loop while 8042 output buffer is empty */
        if (stat & MOUSE_OBF)
        {
            util_sys_inb(MOUSE_OUT_BUF, &data); /* ass. it returns OK */
            if ((stat & (MOUSE_PARITY_ERR | MOUSE_TIMEOUT_ERR)) == 0)
                return data;
            else
                return -1;
        }
        tickdelay(micros_to_ticks(DELAY_US));
    }

    return 1;
}
int mouse_read_response()
{
    return mouse_read_out_buf();
}

int mouse_send_data_to_out_buf(int data)
{
    uint8_t stat;

    util_sys_inb(MOUSE_ST_REG, &stat); /* assuming it returns OK */
    /* loop while 8042 input buffer is not empty */
    int temp = 20;
    while(temp--){
        if ((stat & MOUSE_ST_IBF) == 0)
        {
            sys_outb(MOUSE_CMD_REG, MOUSE_AUX_CMD); /* no args command */
            util_sys_inb(MOUSE_ST_REG, &stat);
            if((stat & MOUSE_ST_IBF) == 0){
                sys_outb(MOUSE_OUT_BUF, data);
                int acknowladgeByte = mouse_read_response();
                if (acknowladgeByte == 0xFA)
                {
                    return 0;
                }
            }
            tickdelay(micros_to_ticks(DELAY_US));
    
            // erros possiveis: 0xFE || 0xFC tratados da mesma forma
        }
    }
    return 1;
}


void mouse_enable_data_reporting_d(){
    int counter = 10;
    while (counter--)
    {
        int ret = mouse_send_data_to_out_buf(0xF4);
        if (ret == 0)
        {
            return;
        }
        tickdelay(micros_to_ticks(DELAY_US));
    }
}
void mouse_disable_data_reporting()
{
    int counter = 10;
    while (counter--)
    {
        int ret = mouse_send_data_to_out_buf(0xF5);
        if (ret == 0)
        {
            return;
        }
        tickdelay(micros_to_ticks(DELAY_US));
    }
}

int mouse_send_command_no_arg(int cmd)
{
    mouse_send_data_to_out_buf(0xF5);
    int counter = 20;
    while (counter--)
    {
        int verifyCMD = mouse_send_data_to_out_buf(cmd);
        if (!verifyCMD)
        {
            mouse_send_data_to_out_buf(0xF4);
            return 0;
        }
        tickdelay(micros_to_ticks(DELAY_US));
    }
    mouse_send_data_to_out_buf(0xF4);
    return 1;
}

int mouse_send_command_with_arg(int cmd, int arg[], int n)
{
    mouse_send_data_to_out_buf(0xF5);
    int counter = 20;
    while (counter--)
    {
        int verifyCMD = mouse_send_data_to_out_buf(cmd);
        if (!verifyCMD)
        {
            bool argsPassed = true;
            for (int i = 0; i < n; i++)
            {
                int verifyARG = mouse_send_data_to_out_buf(arg[i]);
                if (verifyARG)
                {
                    argsPassed = false;
                    break;
                }
            }
            if (argsPassed)
            {
                mouse_send_data_to_out_buf(0xF4);
                return 0;
            }
        }
        tickdelay(micros_to_ticks(DELAY_US));
    }
    mouse_send_data_to_out_buf(0xF4);
    return 1;
}
