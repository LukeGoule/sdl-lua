#include "Engine.h"

#include "engine.Atan2.h"
#include "engine.BindTexture.h"
#include "engine.CheckKey.h"
#include "engine.DrawRect.h"
#include "engine.DrawTriangle.h"
#include "engine.Error.h"
#include "engine.GetMousePosition.h"
#include "engine.LoadTexture.h"
#include "engine.SetDrawColour.h"
#include "engine.SetFullscreen.h"
#include "engine.LaunchWindow.h"
#include "engine.CheckKey.h"

int Engine::initialiseLua(lua_State* L) {

	// Create a new table
	lua_newtable(L);
	
	const auto m_engine = &Engine::getInstance();
	
	engine_LaunchWindow::luaRegister(m_engine);	
	engine_SetDrawColour::luaRegister(m_engine);
	engine_LoadTexture::luaRegister(m_engine);
	engine_DrawTriangle::luaRegister(m_engine);
	engine_DrawRect::luaRegister(m_engine);
	engine_BindTexture::luaRegister(m_engine);
	engine_SetFullscreen::luaRegister(m_engine);
	engine_GetMousePosition::luaRegister(m_engine);
	engine_Error::luaRegister(m_engine);
	engine_Atan2::luaRegister(m_engine);
	engine_CheckKey::luaRegister(m_engine);

	// The table is already on the top of the stack, so return it
	return 1;
}

Engine* Engine::Init() {
	this->m_lua_state = luaL_newstate();
	luaL_openlibs(this->getLua());

	this->m_keyboard = new Keyboard(this);
	this->m_render = new Render(this);

	// Register the test module
	luaL_requiref(this->m_lua_state, "engine", Engine::initialiseLua, 1);
	lua_pop(this->m_lua_state, 1);

	return this;
}

lua_State* Engine::getLua() {
	return this->m_lua_state;
}

Keyboard* Engine::getKeyboard() {
	return this->m_keyboard;
}

Render* Engine::getRender() {
	return this->m_render;
}

Engine* Engine::LoadScript(std::string script) {
	// Load the engine files

	auto fLoad = luaL_loadfile(this->getLua(), script.c_str());

	if (fLoad) {
		this->LuaError("luaL_loadfile");
		return this;
	}

	auto pCall = lua_pcall(this->getLua(), 0, 0, 0);

	if (pCall) {
		this->LuaError("lua_pcall");
		return this;
	}

	return this;
}

Engine* Engine::Error(std::string err) {
	printf("Error caused: %s\n", err.c_str());
	SDL_Quit();

	return this;
}

Engine* Engine::LuaError(std::string trace) {
	printf("[%s][LUA]: %s\n", trace.c_str(), lua_tostring(this->getLua(), -1));
	SDL_Quit();

	return this;
}
