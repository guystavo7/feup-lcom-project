#include "rtc.h"

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
  return (bcd>>4)*10+(0x0F&bcd);

  ; }

int rtc_read_date(rtc_date *date) { 

  int ret;
  u32_t uip;

  ret = sys_outb(RTC_ADDR_REG, RTC_REG_A);
  sys_inb(RTC_DATA_REG,&uip);
  uip=uip&RTC_UIP_MSK;

  while (uip!=0)
  {
    
    tickdelay(micros_to_ticks(20000));
    sys_outb(RTC_ADDR_REG, RTC_REG_A);
    sys_inb(RTC_DATA_REG,&uip);
    uip=uip&RTC_UIP_MSK;
  }
  
  
//   ret = sys_outb(RTC_ADDR_REG, RTC_REG_DAY);
  u32_t dm;

  ret=sys_outb(RTC_ADDR_REG, RTC_REG_B);
  sys_inb(RTC_DATA_REG,&dm);
  dm=dm&RTC_DM_MSK;

  
  
  u32_t day,month,year;


 

  sys_outb(RTC_ADDR_REG,RTC_REG_DAY);
  sys_inb(RTC_DATA_REG,&day);

  sys_outb(RTC_ADDR_REG,RTC_REG_MONTH);
  sys_inb(RTC_DATA_REG,&month);

  sys_outb(RTC_ADDR_REG,RTC_REG_YEAR);
  sys_inb(RTC_DATA_REG,&year);


  if(dm!=0){
    date->day=day;
    date->month=month;
    date->year=year;
  }
  else{
    date->day=bdc_to_bin(day);
    date->month=bdc_to_bin(month);
    date->year=bdc_to_bin(year);
  }
  

  return 0;

  
  ; }
  