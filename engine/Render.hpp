#pragma once

#ifndef RENDER_HPP
#define RENDER_HPP

#include "EngineModule.hpp"
#include "glew.h"

#include <SDL.h>
#include <iostream>

struct SDL_Window;
class TextEditor;

/*
 * Render controller class. This holds the state of OpenGL, and the window, and the bindings between such.
 */
class Render : public EngineModule {
private:
	/*
	 * SDL fullscreen integer mode.
	 */
	int m_fullscreen_mode = 0;
	
	/*
	 * Window width.
	 */
	int m_screen_w = 0;

	/*
	 * Window height.
	 */
	int m_screen_h = 0;

	/*
	 * Pointer to the window object.
	 */
	SDL_Window* m_window = nullptr;

	/*
	 * Pointer to the OpenGL/SDL context.
	 */
	SDL_GLContext m_gl_context = nullptr;

	/*
	 * Should mainloop continue.
	 */
	bool m_continue = true;

	/*
	 * Current vertex shader code.
	 */
	std::string m_sVertex;

	/*
	 * Current fragment shader code.
	 */
	std::string m_sFragment;

	/*
	 * Handle to current compiled shader program.
	 */
	GLuint m_iShaderProgram;

public:
	/*
	 * Initialise the class.
	 */
	Render(Engine* instance);

	/*
	 * Destroy the class.
	 */
	~Render();

	/*
	 * Calls SDL_Init, basically.
	 */
	void initSDL();

	/*
	 * Initialise OpenGL.
	 */
	void initOpenGL();
	
	/*
	 * Create the SDL window.
	 */
	void initWindow(const char* title, int widthPx, int heightPx);

	/*
	 * Initialise the OpenGL state in 2D.
	 */
	void init2d(int widthPx, int heightPx);

	/*
	 * Main loop method.
	 */
	void run2d();

	/*
	 * Shuts down the render.
	 */
	void quit();

	/*
	 * Compiles the shaders stored in m_sFragment and m_sVertex.
	 */
	void recompileShaders(bool bFromFiles = false);

	/*
	 * Compile an OpenGL shader (GLSL).
	 */
	GLuint compileShader(GLenum type, const char* szCode);

	/*
	 * Compile a full OpenGL shader program (GLSL).
	 */
	GLuint compileShaderProgram(const char* szVertexShader, const char* szFragmentShader);

	/*
	 * Returns the SDL fullscreen integer.
	 */
	int getFullscreenMode();

	/*
	 * Sets the SDL fullscreen integer.
	 */
	void setFullscreenMode(int mode);

	/*
	 * Gets the window dimensions.
	 * Returns an int[2].
	 */
	int* getScreenDimensions();

	/*
	 * Gets the pointer to the immutable SDL window object.
	 */
	SDL_Window* getSDLWindow();

	/*
	 * Gets the pointer to the immutable SDL GL context object.
	 */
	SDL_GLContext getSDLGLContext();

	/*
	 * Gets the vertex shader code.
	 */
	std::string getVertexShaderCode();
	
	/*
	 * Gets the fragment shader code.
	 */
	std::string getFragmentShaderCode();
};

#endif