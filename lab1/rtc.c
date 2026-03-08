#include "rtc.h"
#include "minix/syslib.h"
#include <stdio.h>
#include <minix/sysutil.h>

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

static int bcd_to_bin(uint8_t bcd) {
  return (bcd >> 4) * 10 + (bcd & 0x0F);
}

static uint8_t rtc_read_reg(uint8_t reg) {
  uint32_t tmp;
  sys_outb(RTC_ADDR_REG, reg);
  sys_inb(RTC_DATA_REG, &tmp);
  return (uint8_t)tmp;
}

int rtc_read_date(rtc_date *date) {
  uint8_t day, month, year, regB;

  while (rtc_read_reg(RTC_REG_A) & RTC_UIP_MSK)
    tickdelay(micros_to_ticks(20000));

  day = rtc_read_reg(RTC_REG_DAY);
  month = rtc_read_reg(RTC_REG_MONTH);
  year = rtc_read_reg(RTC_REG_YEAR);
  regB = rtc_read_reg(RTC_REG_B);

  if (!(regB & RTC_DM_MSK)) {
    day = bcd_to_bin(day);
    month = bcd_to_bin(month);
    year = bcd_to_bin(year);
  }

  date->day = day;
  date->month = month;
  date->year = year;
  return 0;
}
