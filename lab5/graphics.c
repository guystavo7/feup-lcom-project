/* Set default text mode */
#include <lcom/lcf.h>

#include <stdio.h>
#include <string.h>

int vg_entry(uint16_t mode)
{
    reg86_t reg;
    
    memset(&reg, 0, sizeof(reg));

    reg.intno = 0x10;             // Use INT 0x10
    reg.ah = 0x4F;                // set AH=0x4F
    reg.al = 0x02;                // specify function in AL

    reg.bx = mode | BIT(14);

    if(sys_int86(&reg) != OK){
        printf("\tvg_entry(): sys_int86() failed \n");
        return 1;
    }
    return 0;
}

