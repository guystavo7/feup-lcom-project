#include "rtc.h"
#include <minix/syslib.h>
#include <minix/sysutil.h>
#include <stdbool.h>

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

bool verify(uint8_t info) { return 0 != (info >> 7); }

static int bdc_to_bin(uint8_t bcd) { 
  uint8_t u = bcd & 0x0F;
  uint8_t d = bcd >> 4;
  return (d * 10) + u;
}

int rtc_read_date(rtc_date *date) {
  uint32_t info;
  sys_outb(0x70, 0x0A);
  sys_inb(0x71, &info);
  while (verify(info)) {
    tickdelay(micros_to_ticks(20000));
    sys_outb(0x70, 0x0A);
    sys_inb(0x71, &info);
  }
  uint32_t dataMode;
  sys_outb(0x70, 0x0B);
  sys_inb(0x71, &dataMode);
  dataMode &= 1 << 2;
  dataMode = dataMode >> 2;
  uint32_t year;
  uint32_t month;
  uint32_t day;
  sys_outb(0x70, 0x09);
  sys_inb(0x71, &year);
  sys_outb(0x70, 0x08);
  sys_inb(0x71, &month);
  sys_outb(0x70, 0x07);
  sys_inb(0x71, &day);
  if(!dataMode){
    date->year = bdc_to_bin(year);
    date->month = bdc_to_bin(month);
    date->day = bdc_to_bin(day);
  }
  else{
    date->year = year;
    date->month = month;
    date->day = day;
  }
  
  return 0;
}
