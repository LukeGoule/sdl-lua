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


void Engine::Init() {
	this->L = luaL_newstate();
	luaL_openlibs(this->L);

	this->m_keyboard = new G3Keyboard;

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
}

void Engine::LoadScript(std::string script) {
	// Load the engine files

	auto fLoad = luaL_loadfile(this->Engine::L, script.c_str());

	if (fLoad) {
		this->LuaError("luaL_loadfile");

		return;
	}

	auto pCall = lua_pcall(this->L, 0, 0, 0);

	if (pCall) {
		this->LuaError("lua_pcall");

		return;
	}
}

void Engine::Error(std::string err) {
	printf("Error caused: %s\n", err.c_str());

	SDL_Quit();
}

void Engine::LuaError(std::string trace) {
	printf("[%s][LUA]: %s\n", trace.c_str(), lua_tostring(this->L, -1));

	SDL_Quit();
}
