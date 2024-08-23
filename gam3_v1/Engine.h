#pragma once

#include <iostream>

#include <lua.hpp>
#include <SDL.h>
#include <glew.h>

#include "Keyboard.hpp"

/*
This fully static class is used to hold all of the needed data in a neat, formatted way.
It could be non-static, but Lua bindings would not look good.
*/
class Engine {
public:
	// Core engine variables
	lua_State*		L					= nullptr;
	SDL_Window*		pWin				= nullptr;
	SDL_GLContext	Context				= NULL;
	G3Keyboard*		m_keyboard			= nullptr;
	bool			bShouldContinue		= true;
	int				iFullscreenMode		= 0;

	// SDL & setup stuffs
	void Init();
	void LoadScript(std::string script);

	// Error handling functions
	void Error(std::string err);
	void LuaError(std::string trace);
};