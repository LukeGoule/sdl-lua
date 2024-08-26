#pragma once

#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <iostream>

#include <lua.hpp>
#include <SDL.h>
#include <glew.h>

#include "Keyboard.hpp"
#include "Render.hpp"
#include "Singleton.hpp"
#include "Portaudio.hpp"
#include "Menu.hpp"

/*
 * The render/game/lua/experiment "engine" object. This is a singleton so please do not manually instanciate.
 */
class Engine : public Singleton<Engine> {
private:
	/*
	 * The Lua state variable.
	 */
	lua_State* m_lua_state = nullptr;

	/*
	 * Keyboard controller.
	 */
	Keyboard* m_keyboard = nullptr;

	/*
	 * ImGui men(s) controller.
	 */
	Menu* m_menu = nullptr;

	/*
	 * Render controller.
	 */
	Render* m_render = nullptr;

	/*
	 * Audio controller.
	 */
	Portaudio* m_portaudio = nullptr;

public:

	/*
	 * Returns the pointer to the immutable Lua state.
	 */
	lua_State* getLua();

	/*
	 * Returns the pointer to the immutable keyboard controller.
	 */
	Keyboard* getKeyboard();

	/*
	 * Returns the pointer to the immutable menu controller.
	 */
	Menu* getMenu();

	/*
	 * Returns the pointer to the immutable render controller.
	 */
	Render* getRender();

	/*
	 * Returns the pointer to the immutable audio controller.
	 */
	Portaudio* getPortaudio();

	/*
	 * Setup Lua.
	 */
	Engine* Init();

	/*
	 * Cleanup everything and quit.
	 */
	void Cleanup();
	
	/*
	 * Execute a Lua script on the current thread.
	 */
	Engine* LoadScript(std::string script);

	/*
	 * Trigger an error, by string.
	 */
	Engine* Error(std::string err);

	/*
	 * Trigger an error, with the latest error message reported by Lua.
	 */
	Engine* LuaError(std::string trace);

	/*
	 * Statically callable Lua function, called to include the engine into the current script.
	 * Usage:
	 * ```lua
	 * -- Include
	 * local engine = require("engine");
	 * 
	 * -- Do stuff
	 * print(engine:Atan2(10,10)) -- Prints 0.78539816339745
	 * ```
	 */
	static int initialiseLua(lua_State* L);
};

#endif