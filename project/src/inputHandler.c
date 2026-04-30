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
    enum keyState keys[88];
};

static InputHandler* newInputHandler() {
    InputHandler* i = malloc(sizeof(InputHandler));
    return i;
}

InputHandler* getInputHandler() {
    if (!isInstanceCreated) {
        instance = newInputHandler();
		for (int i = 0; i < 88; i++) instance->keys[i] = KEYUP;
		isInstanceCreated = true;
    }

    return instance;
}

void readKey(InputHandler* inputHandler, uint16_t keycode, bool isMake) {
	enum keyState currentState = inputHandler->keys[keycode - KEYCODE_ESQ];

    if (isMake && currentState == KEYUP) {
        inputHandler->keys[keycode - KEYCODE_ESQ] = KEYDOWN;
    } else if (!isMake){
        inputHandler->keys[keycode - KEYCODE_ESQ] = KEYUP;
    }
}

bool getKeyDown(InputHandler* inputHandler, uint16_t keycode) {
	bool isKeyDown = inputHandler->keys[keycode - KEYCODE_ESQ] == KEYDOWN;
	if (isKeyDown) inputHandler->keys[keycode - KEYCODE_ESQ] = KEYPRESSED;

	return isKeyDown;
}

bool getKeyPressed(InputHandler* inputHandler, uint16_t keycode) {
	return inputHandler->keys[keycode - KEYCODE_ESQ] != KEYUP;
}
