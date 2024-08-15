#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include <SDL.h>

#define KEYBOARD_HPP_KEY_COUNT 255
#define KEYBOARD_HPP_KEY_DOWN 1
#define KEYBOARD_HPP_KEY_UP 0

class G3Keyboard {
public:
	SDL_Keycode *depressedKeys;

	G3Keyboard();
	~G3Keyboard();

	bool checkState(SDL_Keycode keycode);
	void setState(SDL_Keycode keycode, bool isDepressed);
};

#endif