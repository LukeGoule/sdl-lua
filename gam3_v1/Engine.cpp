#include "Engine.h"
#include "EAtan2.h"
#include "EBindTexture.h"
#include "ECheckKey.h"
#include "EDrawRect.h"
#include "EDrawTriangle.h"
#include "EError.h"
#include "EGetMousePosition.h"
#include "ELoadTexture.h"
#include "ESetDrawColour.h"
#include "ESetFullscreen.h"
#include "ESetupWindow.h"
#include "ECheckKey.h"

Engine* Engine::Init() {
	this->m_lua_state = luaL_newstate();
	luaL_openlibs(this->getLua());

	this->m_keyboard = new Keyboard(this);
	this->m_render = new Render(this);

	ESetupWindow::luaRegister(this);
	ESetDrawColour::luaRegister(this);
	ELoadTexture::luaRegister(this);
	EDrawTriangle::luaRegister(this);
	EDrawRect::luaRegister(this);
	EBindTexture::luaRegister(this);
	ESetFullscreen::luaRegister(this);
	EGetMousePosition::luaRegister(this);
	EError::luaRegister(this);
	EAtan2::luaRegister(this);
	ECheckKey::luaRegister(this);

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
