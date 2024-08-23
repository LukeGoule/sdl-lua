#include "ESetFullscreen.h"

std::string ESetFullscreen::getName() {
	return std::string("E_SetFullscreen");
}

int ESetFullscreen::handle(lua_State* L) {
	// SDL_WINDOW_FULLSCREEN, SDL_WINDOW_FULLSCREEN_DESKTOP or 0
	if (lua_gettop(L) != 1) {
		this->m_engine->Error("E_SetFullscreen requires 1 argument\n");
	}

	this->m_engine->getRender()->setFullscreenMode((int)lua_tointeger(L, 1));

	return 1;
}