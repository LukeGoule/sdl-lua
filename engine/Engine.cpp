#include "Engine.hpp"

#include "engine.Atan2.hpp"
#include "engine.BindTexture.hpp"
#include "engine.CheckKey.hpp"
#include "engine.DrawRect.hpp"
#include "engine.DrawTriangle.hpp"
#include "engine.Error.hpp"
#include "engine.GetMousePosition.hpp"
#include "engine.LoadTexture.hpp"
#include "engine.SetDrawColour.hpp"
#include "engine.SetFullscreen.hpp"
#include "engine.LaunchWindow.hpp"
#include "engine.CheckKey.hpp"

int Engine::initialiseLua(lua_State* L) {
	// Create a new table, which will be require-able.
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
	this->m_menu = new Menu(this);
	this->m_render = new Render(this);
	this->m_portaudio = new Portaudio(this);

	// Setup the audio stream.
	this->m_portaudio->initialise();

	// Register the test module
	luaL_requiref(this->m_lua_state, "engine", Engine::initialiseLua, 1);
	lua_pop(this->m_lua_state, 1);

	return this;
}

void Engine::Cleanup() {
	this->m_portaudio->cleanup();
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

lua_State* Engine::getLua() {
	return this->m_lua_state;
}

Keyboard* Engine::getKeyboard() {
	return this->m_keyboard;
}

Menu* Engine::getMenu() {
	return this->m_menu;
}

Render* Engine::getRender() {
	return this->m_render;
}

Portaudio* Engine::getPortaudio() {
	return this->m_portaudio;
}