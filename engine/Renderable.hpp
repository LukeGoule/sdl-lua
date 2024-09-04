#pragma once
#ifndef RENDERABLE_HPP
#define RENDERABLE_HPP

#include <iostream>
#include <vector>
#include <glm/vec3.hpp>
#include <glew.h>
#include <glm/glm.hpp>

#include "IHasUUID.hpp"

class Renderable : public IHasUUID {
private:

	glm::vec3 m_position = { 0.0, 0.0, 0.0 };
	glm::vec3 m_rotation = { 0.0, 0.0, 0.0 };

	std::vector<float> m_vertices;
	std::vector<unsigned int> m_indices;

	glm::mat4 m_matrix;

	GLuint
		VBO = 0, // Vertex buffer object.
		VAO = 0, // Vertex array object
		EBO = 0; // Element buffer object.

	const char* m_szUUID;

public:

	Renderable();

	Renderable* bindBuffers(std::string model = std::string("res/models/cube.obj"));

	void renderBuffers();

	Renderable* setPosition(glm::vec3 pos);

	Renderable* setRotation(glm::vec3 rot);

	glm::vec3 getPosition();

	glm::vec3 getRotation();

	const char* getUUID();
};

#endif