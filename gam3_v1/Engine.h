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
	static lua_State*		L;
	static SDL_Window*		pWin;
	static SDL_GLContext	Context;
	static G3Keyboard*		m_keyboard;
	static bool				bShouldContinue;
	static int				iFullscreenMode;

	// SDL & setup stuffs
	static void Init();
	static void Setup();
	static void LoadScript(std::string script);

	// Error handling functions
	static void Error(std::string err);
	static void LuaError(std::string trace);
	
	// Lua bindings.
	static int	SetupWindow(lua_State* L);
	static int	RenderBinding(lua_State* L);
	static int	SetDrawColour(lua_State* L);
	static int	DrawRect(lua_State* L);
	static int	DrawTriangle(lua_State* L);
	static int	LoadTexture(lua_State* L);
	static int	BindTexture(lua_State* L);
	static int	SetFullscreen(lua_State* L);
	static int	GetMousePosition(lua_State* L);
	static int  Lua_CauseError(lua_State* L);
};