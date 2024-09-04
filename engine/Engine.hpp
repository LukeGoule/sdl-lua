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
#include "Hooks.hpp"
#include "Shaders.hpp"
#include "Uniforms.hpp"
#include "Options.hpp"
#include "Camera.hpp"

#define LUA() (&Engine::getInstance())->getLua()
#define KEYBOARD() (&Engine::getInstance())->getKeyboard()
#define MENU() (&Engine::getInstance())->getMenu()
#define RENDER() (&Engine::getInstance())->getRender()
#define SHADERS() (&Engine::getInstance())->getShaders()
#define UNIFORMS() (&Engine::getInstance())->getUniforms()
#define PORTAUDIO() (&Engine::getInstance())->getPortaudio()
#define HOOKS() (&Engine::getInstance())->getHooks()
#define OPTIONS() (&Engine::getInstance())->getOptions()
#define CAMERA() (&Engine::getInstance())->getCamera()

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
	 * OpenGL shaders controller.
	 */
	Shaders* m_shaders = nullptr;

	/*
	 * OpenGL uniforms controller.
	 */
	Uniforms* m_uniforms = nullptr;

	/*
	 * Audio controller.
	 */
	Portaudio* m_portaudio = nullptr;

	/*
	 * Audio controller.
	 */
	Hooks* m_hooks = nullptr;

	/*
	 * Options controller.
	 */
	Options* m_options = nullptr;

	/*
	 * Camera controller.
	 */
	Camera* m_camera = nullptr;
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
	 * Returns the pointer to the immutable shaders controller.
	 */
	Shaders* getShaders();

	/*
	 * Returns the pointer to the immutable uniforms controller.
	 */
	Uniforms* getUniforms();

	/*
	 * Returns the pointer to the immutable audio controller.
	 */
	Portaudio* getPortaudio();

	/*
	 * Returns the pointer to the immutable options controller.
	 */
	Options* getOptions();

	/*
	 * Returns the pointer to the immutable hooks controller.
	 */
	Hooks* getHooks();

	/*
	 * Returns the pointer to the immutable camera controller.
	 */
	Camera* getCamera();

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