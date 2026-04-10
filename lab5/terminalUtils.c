#include "terminalUtils.h"

int enterMode(uint16_t mode) {
	reg86_t reg; 
	memset(&reg, 0, sizeof(reg));

	reg.intno = VIDEO_CARD_INTERRUPT;
	reg.ah = SET_VBE_MODE_AH;
	reg.al = SET_VBE_MODE_AL;

	reg.bx = mode | LINEAR_FRAME_BUFFER_BIT;

	if (sys_int86(&reg)) {
		printf("Error on setting mode\n");
		return -1;
	}

	return 0;
}

int exitMode() {
  reg86_t r86;
   
  /* Specify the appropriate register values */
  
  memset(&r86, 0, sizeof(r86)); /* zero the structure */

  r86.intno = 0x10;             /* BIOS video services */
  r86.ah = 0x00;                /* Set Video Mode function */
  r86.al = 0x03;                /* 80x25 text mode */
  
  /* Make the BIOS call */

  if( sys_int86(&r86)) {
    printf("\tvg_exit(): sys_int86() failed \n");
    return 1;
  }
  return 0;
}
