//
// Created by greedisland on 3/15/26.
//
#include "keyboard.h"
#include <lcom/lcf.h>

#include <timer.h>
#include <utils.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define KBC_ST_REG 0x64
#define KBC_CMD_REG 0x64
#define KBC_ST_IBF 0x60
#define KBC_OUT_BUF 0x60
#define KBC_PARITY_ERR BIT(7)
#define KBC_TIMEOUT_ERR BIT(6)
#define KBC_OBF BIT(0)

int main(int argc, char *argv[])
{
    // sets the language of LCF messages (can be either EN-US or PT-PT)
    lcf_set_language("EN-US");

    // enables to log function invocations that are being "wrapped" by LCF
    // [comment this out if you don't want/need it]
    lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

    // enables to save the output of printf function calls on a file
    // [comment this out if you don't want/need it]
    lcf_log_output("/home/lcom/labs/lab3/output.txt");

    // handles control over to LCF
    // [LCF handles command line arguments and invokes the right function]
    if (lcf_start(argc, argv))
        return 1;

    // LCF clean up tasks
    // [must be the last statement before return]
    lcf_cleanup();

    return 0;
}

int(kbd_test_scan)()
{
    /* To be completed by the students */
    int ipc_status, r;
    message msg;
    uint8_t bit_no;
    if (kbd_subscribe_int(&bit_no) != 0)
        return 1;
    int irq_set = 1 << bit_no;

    uint8_t bytes[2];
    uint8_t size = 1;
    bool make;

    while (get_scanCode() != 0x81)
    { /* You may want to use a different condition */
        /* Get a request message. */
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0)
        {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status))
        { /* received notification */
            switch (_ENDPOINT_P(msg.m_source))
            {
            case HARDWARE: /* hardware interrupt notification */
                if (msg.m_notify.interrupts & irq_set)
                { /* subscribed interrupt */
                    /* process it */
                    kbc_ih();
                    if (get_readError())
                    {
                        printf("Error reading keyboard\n");
                        continue;
                    }
                    uint8_t currentCode = get_scanCode();
                    if (size == 1)
                    {
                        bytes[0] = currentCode;
                        if (currentCode == 0xE0)
                        {
                            size = 2;
                        }
                        else
                        {
                            make = (currentCode & 1 << 7) == 0;
                            kbd_print_scancode(make, size, bytes);
                        }
                    }
                    else
                    {
                        bytes[1] = currentCode;
                        make = (currentCode & BIT(7)) == 0;
                        kbd_print_scancode(make, size, bytes);
                        size = 1;
                    }
                }
                break;
            default:
                break; /* no other notifications expected: do nothing */
            }
        }
        else
        { /* received a standard message, not a notification */
            /* no standard messages expected: do nothing */
        }
    }

    kbd_unsubscribe_int();
    return 0;
}

int(kbd_test_poll)()
{
    /* To be completed by the students */
    uint8_t bytes[2];
    uint8_t size = 1;
    bool make;

    while (get_scanCode() != 0x81)
    {
        kbc_ih();
        if (get_readError())
        {
            printf("Error reading keyboard\n");
            continue;
        }
        else
        {
            uint8_t currentCode = get_scanCode();
            if (size == 1)
            {
                bytes[0] = currentCode;
                if (currentCode == 0xE0)
                {
                    size = 2;
                }
                else
                {
                    make = (currentCode & (1 << 7)) == 0;
                    kbd_print_scancode(make, size, bytes);
                }
            }
            else
            {
                bytes[1] = currentCode;
                make = (currentCode & BIT(7)) == 0;
                kbd_print_scancode(make, size, bytes);
                size = 1;
            }
        }
    }

    kbc_enable_interrupt();
    return 0;
}

int(kbd_test_timed_scan)(uint8_t n)
{
    /* To be completed by the students */
    int ipc_status, r;
    message msg;
    uint8_t bit_no_timer, bit_no_kbc;
    uint8_t bytes[2];
    uint8_t size = 1;
    bool make;

    if (timer_subscribe_int(&bit_no_timer) != 0)
        return 1;
    if (kbd_subscribe_int(&bit_no_kbc) != 0)
        return 1;
    int irq_set_timer = 1 << bit_no_timer, irq_set_kbc = 1 << bit_no_kbc;
    uint32_t freq = 60;
    int now = 0;
    while ((get_count() - now < n * freq) && get_scanCode() != 0x81)
    {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0)
        {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status))
        { /* received notification */
            switch (_ENDPOINT_P(msg.m_source))
            {
            case HARDWARE: /* hardware interrupt notification */
                if (msg.m_notify.interrupts & irq_set_timer)
                {                        /* subscribed interrupt */
                    timer_int_handler(); /* process it */
                    if (get_count() % freq == 0)
                    {
                        timer_print_elapsed_time(); /*nao deve estar pois nao é uma funcao que pertence a nossa biblioteca */
                    }
                }
                if (msg.m_notify.interrupts & irq_set_kbc)
                { /* subscribed interrupt */
                    /* process it */
                    kbc_ih();
                    now = get_count();
                    if (get_readError())
                    {
                        printf("Error reading keyboard\n");
                        continue;
                    }
                    uint8_t currentCode = get_scanCode();
                    if (size == 1)
                    {
                        bytes[0] = currentCode;
                        if (currentCode == 0xE0)
                        {
                            size = 2;
                        }
                        else
                        {
                            make = (currentCode & 1 << 7) == 0;
                            kbd_print_scancode(make, size, bytes);
                        }
                    }
                    else
                    {
                        bytes[1] = currentCode;
                        make = (currentCode & BIT(7)) == 0;
                        kbd_print_scancode(make, size, bytes);
                        size = 1;
                    }
                    break;
                default:
                    break; /* no other notifications expected: do nothing */
                }
            }
        }
        
    }
    timer_unsubscribe_int();
    kbd_unsubscribe_int();
    return 0;
}


