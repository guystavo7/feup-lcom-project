#include "../include/cursorHandler.h"
#include <stdint.h>

static int u = 0, v = 0;
int width, height;
int center[2];

void cursorInit(int w, int h){
  center[0] = w/2;
  center[1] = h/2;
  width = w;
  height = h;
}

void cursorReset(){
  u = 0;
  v = 0;
}

void cursorUpdateCoordinates(int16_t du, int16_t dv){
  int nextX = u + du;
  int nextY = v + dv;

  int pu = center[0] + nextX;
  if (pu < 0) u = -center[0];
  else if (pu > width - 1) u = (width - 1) - center[0];
  else u = nextX;

  int pv = center[1] + nextY;
  if (pv < 0) v = -center[1];
  else if (pv > height - 1) v = (height - 1) - center[1];
  else v = nextY;
}

void cursorSpaceToPixel(int* pu, int* pv){
  *pu = center[0] + u;
  *pv = center[1] + v;
}

int cursorGetU(){
  return u;
}

int cursorGetV(){
  return v;
}

