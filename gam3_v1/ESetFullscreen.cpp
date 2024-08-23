#include "ESetFullscreen.h"

std::string ESetFullscreen::getName() {
	return std::string("E_SetFullscreen");
}

int ESetFullscreen::handle(lua_State* L) {
	// SDL_WINDOW_FULLSCREEN, SDL_WINDOW_FULLSCREEN_DESKTOP or 0

	if (lua_gettop(L) != 1) {
		this->m_engine->Error("SetFullscreen requires 1 argument\n");
	}

	int OldMode = this->m_engine->iFullscreenMode;

	this->m_engine->iFullscreenMode = (int)lua_tointeger(L, 1);

	if (!(this->m_engine->iFullscreenMode == 0 || this->m_engine->iFullscreenMode == SDL_WINDOW_FULLSCREEN || this->m_engine->iFullscreenMode == SDL_WINDOW_FULLSCREEN_DESKTOP)) {
		this->m_engine->Error("Invalid fullscreen mode passed!\n");
	}

	SDL_SetWindowFullscreen(this->m_engine->pWin, this->m_engine->iFullscreenMode);

	return 1;
}