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

#include "LuaImGuiModule.hpp"
#include "LuaHooksModule.hpp"

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
	this->m_shaders = new Shaders(this);
	this->m_uniforms = new Uniforms(this);
	this->m_portaudio = new Portaudio(this);
	this->m_hooks = new Hooks(this);
	this->m_options = new Options(this);

	// Setup the audio stream.
	this->m_portaudio->initialise();

	// Register the test module
	luaL_requiref(this->m_lua_state, "engine", Engine::initialiseLua, 1);
	lua_pop(this->m_lua_state, 1);

	(&LuaImGuiModule::getInstance())->addModule(this);
	(&LuaHooksModule::getInstance())->addModule(this);

	return this;
}

void Engine::Cleanup() {
	SDL_Quit();

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
	this->Cleanup();

	return this;
}

Engine* Engine::LuaError(std::string trace) {
	printf("[%s][LUA]: %s\n", trace.c_str(), lua_tostring(this->getLua(), -1));
	this->Cleanup();

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

Shaders* Engine::getShaders() {
	return this->m_shaders;
}

Uniforms* Engine::getUniforms() {
	return this->m_uniforms;
}

Portaudio* Engine::getPortaudio() {
	return this->m_portaudio;
}

Options* Engine::getOptions() {
	return this->m_options;
}

Hooks* Engine::getHooks() {
	return this->m_hooks;
}