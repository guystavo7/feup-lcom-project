#include "../include/inputHandler.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

enum keyState {
    KEYUP,
    KEYDOWN,
    KEYPRESSED
};

struct InputHandler {
    enum keyState keys[3];
};

static InputHandler* newInputHandler() {
    InputHandler* i = malloc(sizeof(InputHandler));
    return i;
}

InputHandler* getInstance() {
    if (!isInstanceCreated) {
        instance = newInputHandler();
		for (int i = 0; i < 3; i++) instance->keys[i] = KEYUP;
		isInstanceCreated = true;
    }

    return instance;
}

void readKey(InputHandler* inputHandler, uint16_t keycode, bool isMake) {
	if (keycode > 0x04) return;
	enum keyState currentState = inputHandler->keys[keycode - KEYCODE_1];

    if (isMake && currentState == KEYUP) {
        inputHandler->keys[keycode - KEYCODE_1] = KEYDOWN;
    } else if (!isMake){
        inputHandler->keys[keycode - KEYCODE_1] = KEYUP;
    }
}

bool getKeyDown(InputHandler* inputHandler, uint16_t keycode) {
	bool isKeyDown = inputHandler->keys[keycode - KEYCODE_1] == KEYDOWN;
	if (isKeyDown) inputHandler->keys[keycode - KEYCODE_1] = KEYPRESSED;

	return isKeyDown;
}

bool getKeyPressed(InputHandler* inputHandler, uint16_t keycode) {
	return inputHandler->keys[keycode - KEYCODE_1] != KEYUP;
}
