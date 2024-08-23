#include "ESetupWindow.h"

std::string ESetupWindow::getName() {
	return std::string("E_LaunchWindow");
}

int ESetupWindow::handle(lua_State* L) {
	int			wx = lua_tonumber(L, 1);
	int			wy = lua_tonumber(L, 2);
	const char* wn = lua_tostring(L, 3);

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	this->m_engine->pWin = SDL_CreateWindow(wn, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, wx, wy, SDL_WINDOW_OPENGL);
	if (this->m_engine->pWin == NULL) {
		printf("SDL_Error: %s\n", SDL_GetError());
		return 1;
	}

	this->m_engine->Context = SDL_GL_CreateContext(this->m_engine->pWin);
	if (this->m_engine->Context == NULL) {
		printf("SDL_Error: %s\n", SDL_GetError());
		return 0;
	}

	lua_getglobal(L, "Engine_LoadResources");

	if (lua_pcall(L, 0, 0, 0) != 0) {
		printf("[LUA], Engine_Render: %s\n", lua_tostring(L, -1));
		this->m_engine->bShouldContinue = false;
	}

	glewExperimental = GL_TRUE;
	glewInit();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glOrtho(0, wx, wy, 0, -1, 1);

	while (this->m_engine->bShouldContinue) {
		SDL_Event* currEvents = new SDL_Event;
		while (SDL_PollEvent(currEvents)) {
			switch (currEvents->type) {
			case SDL_EventType::SDL_QUIT: // app quit signal.
				this->m_engine->bShouldContinue = false;
				break;
			case SDL_EventType::SDL_KEYDOWN: // keyboard handler up.
				this->m_engine->m_keyboard->setState(currEvents->key.keysym.sym, true);
				break;
			case SDL_EventType::SDL_KEYUP: // keyboard handler down.
				this->m_engine->m_keyboard->setState(currEvents->key.keysym.sym, false);
				break;
			}
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0, 0, 0, 1);

		lua_getglobal(L, "Engine_Render");

		if (lua_pcall(L, 0, 0, 0) != 0) {
			printf("[LUA], Engine_Render: %s\n", lua_tostring(L, -1));
			this->m_engine->bShouldContinue = false;
		}

		SDL_GL_SwapWindow(this->m_engine->pWin);
	}

	SDL_Quit();

	return 0;
}