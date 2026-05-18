#include "../include/button.h"

#define BUTTON_X 20
#define BUTTON_Y 20
#define BUTTON_W 120
#define BUTTON_H 40

#define POPUP_X 250
#define POPUP_Y 175
#define POPUP_W 300
#define POPUP_H 200

#define COLOR_BUTTON_FILL 0x00CCCCCC
#define COLOR_BUTTON_BORDER 0x00FFFFFF
#define COLOR_POPUP_FILL 0x00333333
#define COLOR_POPUP_BORDER 0x00FFFFFF

static bool popupVisible = false;


static bool pointInRect(int px, int py, int x, int y, int w, int h) {
    return px >= x && px < x + w && py >= y && py < y + h;
}

static void drawBorderedRect(Terminal* terminal, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t fill,
                            uint32_t border) {
    drawRectangle(terminal, x, y, w, h, fill);

    if (w < 2 || h < 2) return; //nao desenha borda quando é pequeno

    drawLine(terminal, x, y, w, border);
    drawLine(terminal, x, y + h - 1, w, border);

    for (uint16_t j = y; j < y + h; j++) {
        drawPixel(terminal, x, j, border);
        drawPixel(terminal, x + w - 1, j, border);
    }
}

void buttonInit(void) {
    popupVisible = false;
}

void buttonFree(void) {}

void buttonDraw(Terminal* terminal) {

    drawBorderedRect(terminal, BUTTON_X, BUTTON_Y, BUTTON_W, BUTTON_H, COLOR_BUTTON_FILL, COLOR_BUTTON_BORDER);

    if (popupVisible)
        drawBorderedRect(terminal, POPUP_X, POPUP_Y, POPUP_W, POPUP_H, COLOR_POPUP_FILL, COLOR_POPUP_BORDER);
}

void buttonOnMouseClick(int mx, int my) {
    if (pointInRect(mx, my, BUTTON_X, BUTTON_Y, BUTTON_W, BUTTON_H)) {
        popupVisible = true;
        return;
    }

    if (popupVisible && !pointInRect(mx, my, POPUP_X, POPUP_Y, POPUP_W, POPUP_H)) popupVisible = false;
}

bool buttonIsPopupOpen(void) {
    return popupVisible;
}
