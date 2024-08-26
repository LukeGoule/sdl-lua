#include "engine.SetFullscreen.hpp"

std::string engine_SetFullscreen::getName() {
	return std::string("SetFullscreen");
}

int engine_SetFullscreen::handle(lua_State* L) {
	// SDL_WINDOW_FULLSCREEN, SDL_WINDOW_FULLSCREEN_DESKTOP or 0
	if (lua_gettop(L) != 2) {
		this->m_engine->Error("SetFullscreen requires 1 argument\n");
	}

	this->m_engine->getRender()->setFullscreenMode((int)lua_tointeger(L, 2));

	return 1;
}