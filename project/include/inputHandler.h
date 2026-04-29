#ifndef INPUT_HANDLER
#define INPUT_HANDLER

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdbool.h>

#define KEYCODE_1 2 
#define KEYCODE_2 3 
#define KEYCODE_3 4 
#define KEYCODE_4 5
#define KEYCODE_5 6
#define KEYCODE_6 7
#define KEYCODE_7 8
#define KEYCODE_8 9
#define KEYCODE_9 10
#define KEYCODE_0 11

#define KEYCODE_Q 16
#define KEYCODE_W 17
#define KEYCODE_E 18
#define KEYCODE_R 19
#define KEYCODE_T 20 
#define KEYCODE_Y 21 
#define KEYCODE_U 22
#define KEYCODE_I 23
#define KEYCODE_O 24
#define KEYCODE_P 25

#define KEYCODE_A 30 
#define KEYCODE_S 31
#define KEYCODE_D 32
#define KEYCODE_F 33
#define KEYCODE_G 34
#define KEYCODE_H 35
#define KEYCODE_J 36
#define KEYCODE_K 37
#define KEYCODE_L 38

#define KEYCODE_Z 44 
#define KEYCODE_X 45
#define KEYCODE_C 46
#define KEYCODE_V 47
#define KEYCODE_B 48
#define KEYCODE_N 49
#define KEYCODE_M 50

#define KEYCODE_ESQ 1
#define KEYCODE_BACKSPACE 14
#define KEYCODE_TAB 15
#define KEYCODE_ENTER 28
#define KEYCODE_LCTRL 29
#define KEYCODE_LSHIFT 42
#define KEYCODE_RSHIFT 54
#define KEYCODE_ALT 56
#define KEYCODE_SPACE 57


typedef struct InputHandler InputHandler;

static InputHandler* instance;
static bool isInstanceCreated = false;

InputHandler* getInstance();
bool getKeyDown(InputHandler* inputHandler, uint16_t keycode);
bool getKeyPressed(InputHandler* inputHandler, uint16_t keycode);
void readKey(InputHandler* inputHandler, uint16_t keycode, bool isMake);

#endif
