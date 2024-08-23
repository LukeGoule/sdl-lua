#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include <SDL.h>

#include "EngineModule.h"

#define KEYBOARD_HPP_KEY_COUNT 255
#define KEYBOARD_HPP_KEY_DOWN 1
#define KEYBOARD_HPP_KEY_UP 0

class Keyboard : public EngineModule {
public:
	SDL_Keycode *m_depressed_keys;

	Keyboard(Engine* instance);
	~Keyboard();
	
	bool checkState(SDL_Keycode keycode);
	void setState(SDL_Keycode keycode, bool isDepressed);
};

#endif