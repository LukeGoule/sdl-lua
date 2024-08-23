#pragma once

#ifndef RENDER_H
#define RENDER_H

#include "EngineModule.h"
#include <SDL.h>

struct SDL_Window;

class Render : public EngineModule {
private:
	int m_fullscreen_mode = 0;
	int m_screen_w = 0;
	int m_screen_h = 0;
	SDL_Window* m_window = nullptr;
	SDL_GLContext m_gl_context = nullptr;

	bool m_continue = true;

public:
	Render(Engine* instance);
	~Render();

	void initSDL();
	void initOpenGL();
	void initWindow(const char* title, int widthPx, int heightPx);
	void init2d(int widthPx, int heightPx);
	void run();
	void quit();

	int getFullscreenMode();
	void setFullscreenMode(int mode);

	int* getScreenDimensions();

	SDL_Window* getSDLWindow();
	SDL_GLContext getSDLGLContext();
};

#endif