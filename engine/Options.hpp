#pragma once

#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include "EngineModule.hpp"

#include <iostream>
#include <vector>

#include "glew.h"
#include "glm/glm.hpp"

/*
 *
 * Engine options controller.
 *
 */
class Options : public EngineModule {
private:

	struct Options_t {
		glm::vec2 m_vecCenter = { 0.f, 0.f };
		float m_fZoom = 400.f;
		int m_iIterations = 1000;

		float m_fFOV = 45.f;

		glm::vec3 m_vecViewpos = glm::vec3(0.0f, 0.0f, .0f);
		glm::vec3 m_vecViewrot = glm::vec3(0.f, 0.f, 0.f);
		glm::vec3 m_lightPosition = glm::vec3(1.f, 1.f, 1.f);
		glm::vec3 m_lightColour = glm::vec3(.5f, .5f, .5f);
	};

	Options::Options_t* m_pOptions = nullptr;

public:

	/*
	 * Constructor.
	 */
	Options(Engine* instance);

	/*
	 * Destroy the object.
	 */
	~Options();

	/*
	 * Initialise shaders.
	 */
	void init();

	/*
	 * Get stored options instance.
	 */
	Options::Options_t* get();
};

#endif // !OPTIONS_HPP