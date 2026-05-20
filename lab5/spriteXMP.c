#include "spriteXMP.h"
#include "terminal.h"
#include <stdint.h>

struct SpriteXPM {
    xpm_image_t* img;
	int offsetX;
	int offsetY;

    uint8_t* pixMap;
};

SpriteXPM* newSpriteXPM(xpm_map_t map, enum xpm_image_type type, int offsetX, int offsetY) {
    xpm_image_t* img = malloc(sizeof(xpm_image_t));
    uint8_t* pixMap = xpm_load(map, type, img);

    SpriteXPM* sprite = malloc(sizeof(SpriteXPM));
    sprite->img = img;
    sprite->pixMap = pixMap;
	sprite->offsetX = offsetX;
	sprite->offsetY = offsetY;

    return sprite;
}

void freeSpriteXPM(SpriteXPM* sprite) {
    free(sprite->pixMap);
    free(sprite->img);
    free(sprite);
}

static uint32_t getColorFromPixel(SpriteXPM* sprite, unsigned int bytesPerPixel, int x, int y) {
    uint32_t color = 0;
    long currentX = x * bytesPerPixel;
    long offset = currentX + y * bytesPerPixel * sprite->img->width;

    for (int i = bytesPerPixel - 1; i >= 0; i--) color = (color << 8) | sprite->pixMap[offset + i];
    return color;
}

void drawSpriteXPM(SpriteXPM* sprite, Terminal* terminal, int x, int y) {
    for (uint16_t i = 0; i < sprite->img->height; i++) {
        for (uint16_t j = 0; j < sprite->img->width; j++) {
            drawPixel(terminal, x + sprite->offsetX + j, y + sprite->offsetY + i, getColorFromPixel(sprite, getBytesPerPixel(terminal), j, i));
        }
    }
}

enum xpm_image_type getSpriteType(SpriteXPM* sprite) {
    return sprite->img->type;
}

uint16_t getSpriteWidth(SpriteXPM* sprite) {
    return sprite->img->width;
}

uint16_t getSpriteHeight(SpriteXPM* sprite) {
    return sprite->img->height;
}

size_t getSpriteSize(SpriteXPM* sprite) {
    return sprite->img->size;
}
