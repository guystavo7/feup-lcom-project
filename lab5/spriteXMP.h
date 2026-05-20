#ifndef SPRITE_XPM 
#define SPRITE_XPM 

#include <lcom/lcf.h>
#include "terminal.h"

typedef struct SpriteXPM SpriteXPM;

SpriteXPM* newSpriteXPM(xpm_map_t xpm, enum xpm_image_type type, int offsetX, int offsetY);
void freeSpriteXPM(SpriteXPM* sprite); 
void drawSpriteXPM(SpriteXPM* sprite, Terminal* terminal, int x, int y);

enum xpm_image_type getSpriteType(SpriteXPM* sprite);
uint16_t getSpriteWidth(SpriteXPM* sprite);
uint16_t getSpriteHeight(SpriteXPM* sprite);
size_t getSpriteSize(SpriteXPM* sprite);
uint8_t getSpritePixel(SpriteXPM* sprite, uint16_t x, uint16_t y);

#endif
