#ifndef BUTTON
#define BUTTON

#include "../../lab5/spriteXMP.h"
#include "../../lab5/terminal.h"

#include <lcom/xpm.h>

#include <stdbool.h>
#include <stdint.h>

typedef struct Button Button;
typedef struct Popup Popup;
typedef struct UIManager UIManager;

typedef void (*ButtonClickFn)(Button* self);

Button* buttonNew(xpm_map_t xpm, uint16_t x, uint16_t y, ButtonClickFn onClick);
void buttonFree(Button* b);
void buttonDraw(Button* b, Terminal* t);

Popup* popupNew(xpm_map_t xpm, uint16_t x, uint16_t y);
void popupFree(Popup* p);
void popupOpen(Popup* p);
void popupClose(Popup* p);
bool popupIsOpen(Popup* p);
void popupDraw(Popup* p, Terminal* t);

UIManager* uiManagerNew(void);
void uiManagerFree(UIManager* ui);
void uiManagerAddButton(UIManager* ui, Button* b);
void uiManagerAddPopup(UIManager* ui, Popup* p);
void uiManagerDraw(UIManager* ui, Terminal* t);
void uiManagerHandleClick(UIManager* ui, int mx, int my);

#endif
