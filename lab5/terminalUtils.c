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
