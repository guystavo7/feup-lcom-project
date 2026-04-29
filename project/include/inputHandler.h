#ifndef INPUT_HANDLER
#define INPUT_HANDLER

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdbool.h>

#define KEYCODE_1 0x02 
#define KEYCODE_2 0x03 
#define KEYCODE_3 0x04 

typedef struct InputHandler InputHandler;

static InputHandler* instance;
static bool isInstanceCreated = false;

InputHandler* getInstance();
bool getKeyDown(InputHandler* inputHandler, uint16_t keycode);
bool getKeyPressed(InputHandler* inputHandler, uint16_t keycode);
void readKey(InputHandler* inputHandler, uint16_t keycode, bool isMake);

#endif
