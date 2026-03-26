#include "bitwise.h"
#include <stdarg.h>
#include "stdio.h"

#define TODO return 255

uint8_t clear(uint8_t msk, int pos) { 
  int mask=1<<pos;
  msk&=~mask;
  return msk;

  ; }

uint8_t set(uint8_t msk, int pos) { 
  int mask=1<<pos;
  msk|=mask;
  return msk;
   }

bool is_set(uint8_t msk, int pos) { 
  int mask=1<<pos;
  return mask&msk;
  
}

uint8_t lsb(uint16_t wide_msk) { 
  int mask=0x00FF;
  return mask&wide_msk;
  ; }

uint8_t msb(uint16_t wide_msk) { 
  
  int mask=0xFF00;
  return (wide_msk&mask)>>8;
  ; }

uint8_t mask(int pos, ...) { 
  va_list list;
  uint8_t msk=0;
  va_start(list,pos);

  msk=set(msk,pos);

  int val=va_arg(list,int);
  while (val!=MSK_END){
    msk=set(msk,val);
    //printf("%d\n",val);
    //fflush(stdout);
    val=va_arg(list,int);

  }
  
  
  va_end(list);


  return msk;

}
