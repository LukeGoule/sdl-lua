#include "Keyboard.hpp"

#include <stdio.h>

G3Keyboard::G3Keyboard() {
	this->depressedKeys = new SDL_Keycode[KEYBOARD_HPP_KEY_COUNT];
	for (size_t i = 0; i < KEYBOARD_HPP_KEY_COUNT; i++) {
		this->depressedKeys[i] = KEYBOARD_HPP_KEY_UP;
	}
}

G3Keyboard::~G3Keyboard() {
	delete this->depressedKeys; // I don't remember how to code C++ lmao. (15/08/2024)
}

bool G3Keyboard::checkState(SDL_Keycode keycode) {
	return this->depressedKeys[keycode] == KEYBOARD_HPP_KEY_DOWN;
}

void G3Keyboard::setState(SDL_Keycode keycode, bool isDepressed) {
	this->depressedKeys[keycode] = isDepressed ? KEYBOARD_HPP_KEY_DOWN : KEYBOARD_HPP_KEY_UP;
}