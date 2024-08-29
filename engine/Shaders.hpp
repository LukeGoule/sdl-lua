#pragma once

#ifndef SHADERS_HPP
#define SHADERS_HPP

#include "EngineModule.hpp"

#include <iostream>
#include <vector>
#include <glew.h>

/*
 *
 * OpenGL shaders controller.
 *
 */
class Shaders : public EngineModule {
private:

public:

	/*
	 * Constructor.
	 */
	Shaders(Engine* instance);

	/*
	 * Destroy the object.
	 */
	~Shaders();

	/*
	 * Initialise shaders.
	 */
	void init();

	/*
	 * Compile an OpenGL shader (GLSL).
	 */
	GLuint compileShader(GLenum type, const char* szCode);

	/*
	 * Compile a full OpenGL shader program (GLSL).
	 */
	GLuint compileShaderProgram(const char* szVertexShader, const char* szFragmentShader);
};

#endif // !SHADERS_HPP