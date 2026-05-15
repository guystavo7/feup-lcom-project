#include "../include/cursorHandler.h"
#include <stdint.h>

struct cursor{
  int x, y;
  int width, height;
  int center[2];
};

static struct cursor c;

void cursorInit(int w, int h){
  c.center[0] = w/2;
  c.center[1] = h/2;
  c.x = 0;
  c.y = 0;
  c.width = w;
  c.height = h;
}

void cursorReset(){
  c.x = 0;
  c.y = 0;
}

void cursorUpdateCoordinates(int16_t dx, int16_t dy){
  int nextX = c.x + dx;
  int nextY = c.y + dy;

  int px = c.center[0] + nextX;
  if (px < 0) c.x = -c.center[0];
  else if (px > c.width - 1) c.x = (c.width - 1) - c.center[0];
  else c.x = nextX;

  int pv = c.center[1] + nextY;
  if (pv < 0) c.y = -c.center[1];
  else if (pv > c.height - 1) c.y = (c.height - 1) - c.center[1];
  else c.y = nextY;
}

void cursorSpaceToPixel(int* px, int* py){

  *px = c.center[0] + c.x;
  *py = c.center[1] + c.y;

}

int cursorGetX(){
  return c.x;
}

int cursorGetY(){
  return c.y;
}

