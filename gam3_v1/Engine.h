#pragma once

#include <iostream>

#include <lua.hpp>
#include <SDL.h>
#include <glew.h>

#include "Keyboard.h"
#include "Render.h"

class Engine {
private:
	lua_State* m_lua_state = nullptr;
	Keyboard* m_keyboard = nullptr;
	Render* m_render = nullptr;
public:
	lua_State* getLua();
	Keyboard* getKeyboard();
	Render* getRender();

	// SDL & setup stuffs
	Engine* Init();
	Engine* LoadScript(std::string script);

	// Error handling functions
	Engine* Error(std::string err);
	Engine* LuaError(std::string trace);
};