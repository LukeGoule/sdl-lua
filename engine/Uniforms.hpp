#pragma once

#ifndef UNIFORMS_HPP
#define UNIFORMS_HPP

#include "EngineModule.hpp"

#include <iostream>
#include <vector>

#include "glew.h"

/*
 *
 * OpenGL uniform values controller.
 *
 */
class Uniforms : public EngineModule {
public:

	enum UniformType {
		FLOAT1,
		INT1,

		FLOAT2,
		INT2,
		MAT2,

		FLOAT3,
		INT3,
		MAT3,

		FLOAT4,
		INT4,
		MAT4
	};

	struct Uniform_t {
		Uniforms::UniformType m_type;
		void* m_pData;
		std::string m_name;
	};

public:

	std::vector<Uniforms::Uniform_t*> m_vecUniforms;

	/*
	 * Constructor.
	 */
	Uniforms(Engine* instance);

	/*
	 * Destroy the object.
	 */
	~Uniforms();

	/*
	 * Initialise uniforms.
	 */
	void init();

	/*
	 * Register a new uniform with a pointer to a variable.
	 */
	Uniforms* addUniform(Uniforms::Uniform_t* pUniform);
	Uniforms* addUniform(Uniforms::UniformType type, void* pData, std::string name);

	/*
	 * Method to set the values of the uniforms prior to the execution of the shader program.
	 */
	void setOpenGLUniforms(GLuint iShaderProgram);

	/*
	 * setOpenGLUniforms calls this in bulk.
	 */
	void setSingleUniform(GLuint iShaderProgram, Uniforms::Uniform_t* pUniform);
};

#endif // !UNIFORMS_HPP