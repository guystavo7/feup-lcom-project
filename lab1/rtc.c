#include "rtc.h"
#include <minix/syslib.h>
#include <stdio.h>

#define TODO return -1

#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71
#define RTC_REG_A 0x0A
#define RTC_REG_B 0x0B
#define RTC_REG_DAY 0x07
#define RTC_REG_MONTH 0x08
#define RTC_REG_YEAR 0x09
#define RTC_UIP_MSK (1 << 7)
#define RTC_DM_MSK (1 << 2)

static int bdc_to_bin(uint8_t bcd) {
	return (bcd >> 4) * 10 + (bcd & 0x0F);
}

int getUIP() {
	uint32_t uip;

	int ret = sys_outb(RTC_ADDR_REG, RTC_REG_A);
	sys_inb(RTC_DATA_REG, &uip);
	return uip & RTC_UIP_MSK;
}

uint8_t readRTC(uint8_t reg) {
	int uip = getUIP();
	while (uip) {
		uip = getUIP();
	}

	uint32_t value;
	sys_outb(RTC_ADDR_REG, reg);
	sys_inb(RTC_DATA_REG, &value);

	return value;
}

int rtc_read_date(rtc_date *date) {
	uint8_t day = readRTC(RTC_REG_DAY), month = readRTC(RTC_REG_MONTH), year = readRTC(RTC_REG_YEAR);
	uint8_t isBDC = readRTC(RTC_REG_B);

	// RTC_DM_MSK is wrong???
	if (isBDC & (RTC_DM_MSK >> 1)) {
		day = bdc_to_bin(day);
		month = bdc_to_bin(month);
		year = bdc_to_bin(year);
	}

	date -> day = day;
	date -> month = month;
	date -> year = year;

	return 1;
}
