#include "Keyboard.hpp"
#include <stdio.h>

Keyboard::Keyboard( Engine* instance ) : EngineModule(instance) {
	this->m_depressed_keys = new SDL_Keycode[KEYBOARD_HPP_KEY_COUNT];

	for (size_t i = 0; i < KEYBOARD_HPP_KEY_COUNT; i++) {
		this->m_depressed_keys[i] = KEYBOARD_HPP_KEY_UP;
	}
}

Keyboard::~Keyboard() {
	delete this->m_depressed_keys; // @TODO Check this is valid.
}

bool Keyboard::checkState(SDL_Keycode keycode) {
	return this->m_depressed_keys[keycode] == KEYBOARD_HPP_KEY_DOWN;
}

void Keyboard::setState(SDL_Keycode keycode, bool isDepressed) {
	this->m_depressed_keys[keycode] = isDepressed ? KEYBOARD_HPP_KEY_DOWN : KEYBOARD_HPP_KEY_UP;
}