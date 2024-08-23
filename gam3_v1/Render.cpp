#include "Render.h"
#include "Engine.h"

#include <iostream>

Render::Render(Engine* instance) : EngineModule(instance) {}

Render::~Render() {}

void Render::initSDL() {
	SDL_Init(SDL_INIT_EVERYTHING);
}

void Render::initOpenGL() {
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
}

void Render::initWindow(const char* title, int widthPx, int heightPx) {
	this->m_screen_w = widthPx;
	this->m_screen_h = heightPx;
	this->m_window = SDL_CreateWindow(title , SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, widthPx, heightPx, SDL_WINDOW_OPENGL);
	
	if (this->m_window == NULL) {
		this->m_engine->Error(std::string("SDL_Error: ") + std::string(SDL_GetError()));
	}
}

void Render::init2d(int widthPx, int heightPx) {
	this->m_gl_context = SDL_GL_CreateContext(this->m_window);
	if (this->m_gl_context == NULL) {
		this->m_engine->Error(std::string("SDL_Error: ") + std::string(SDL_GetError()));
	}
}

void Render::run() {
	lua_getglobal(this->m_engine->getLua(), "Engine_LoadResources");

	if (lua_pcall(this->m_engine->getLua(), 0, 0, 0) != 0) {
		printf("[LUA], Engine_Render: %s\n", lua_tostring(this->m_engine->getLua(), -1));
		this->m_continue = false;
	}

	glewExperimental = GL_TRUE;
	glewInit();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glOrtho(0, this->m_screen_w, this->m_screen_h, 0, -1, 1);

	while (this->m_continue) {
		SDL_Event* currEvents = new SDL_Event;
		while (SDL_PollEvent(currEvents)) {
			switch (currEvents->type) {
			case SDL_EventType::SDL_QUIT: // app quit signal.
				this->m_continue = false;
				break;
			case SDL_EventType::SDL_KEYDOWN: // keyboard handler up.
				this->m_engine->getKeyboard()->setState(currEvents->key.keysym.sym, true);
				break;
			case SDL_EventType::SDL_KEYUP: // keyboard handler down.
				this->m_engine->getKeyboard()->setState(currEvents->key.keysym.sym, false);
				break;
			}
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0, 0, 0, 1);

		lua_getglobal(this->m_engine->getLua(), "Engine_Render");

		if (lua_pcall(this->m_engine->getLua(), 0, 0, 0) != 0) {
			printf("[LUA], Engine_Render: %s\n", lua_tostring(this->m_engine->getLua(), -1));
			this->m_continue = false;
		}

		SDL_GL_SwapWindow(this->m_window);
	}
}

void Render::quit() {
	SDL_Quit();
}

int Render::getFullscreenMode() {
	return this->m_fullscreen_mode;
}

void Render::setFullscreenMode(int mode) {
	this->m_fullscreen_mode = mode;
	SDL_SetWindowFullscreen(this->getSDLWindow(), this->m_fullscreen_mode);
}

int* Render::getScreenDimensions() {
	int* dims = new int[2];
	dims[0] = this->m_screen_w;
	dims[1] = this->m_screen_h;
	return dims;
}

SDL_Window* Render::getSDLWindow() {
	return this->m_window;
}

SDL_GLContext Render::getSDLGLContext() {
	return this->m_gl_context;
}