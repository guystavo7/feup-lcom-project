#include <lcom/lcf.h>
#include <stdint.h>

#define LSB_MSK 0x00FF
#define MSB_MSK 0xFF00

int(util_get_LSB)(uint16_t val, uint8_t* lsb) {
	*lsb = (uint8_t) val & LSB_MSK;
    return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t* msb) {
	*msb = (val & MSB_MSK) >> 8;
	return 0;
}

int(util_sys_inb)(int port, uint8_t* value) {
    uint32_t read;
    if (sys_inb(port, &read)) {
		return -1;
	}

    *value = (uint8_t)read;
    return 0;
}
