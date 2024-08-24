#pragma once

#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include <SDL.h>
#include "EngineModule.hpp"

/*
 * The max key-code we can track. This also defines the length of m_depressed_keys in any Keyboard object.
 */
constexpr auto KEYBOARD_HPP_KEY_COUNT = 255;

/*
 * Value used to represent a currently pressed key.
 */
constexpr auto KEYBOARD_HPP_KEY_DOWN = 1;

/*
 * Value used to represent a not currently pressed key.
 */
constexpr auto KEYBOARD_HPP_KEY_UP = 0;


/*
 * 
 * Our keyboard controller class. This handles the states of the keys on your keyboard.
 *
 */
class Keyboard : public EngineModule {
private:
	/*
	 * The state of our keys. On instanciation, this class will allocate sizeof(SDL_Keycode) * KEYBOARD_HPP_KEY_COUNT for the keys.
	 */
	SDL_Keycode* m_depressed_keys;

public:

	/*
	 * Constructor for the keyboard controller. Allocates memory for the key states.
	 */
	Keyboard(Engine* instance);

	/*
	 * De-allocate the memory for the key states.
	 */
	~Keyboard();
	
	/*
	 * Check the state of a given key-code.
	 */
	bool checkState(SDL_Keycode keycode);

	/*
	 * Set the state of a given key-code.
	 */
	void setState(SDL_Keycode keycode, bool isDepressed);
};

#endif