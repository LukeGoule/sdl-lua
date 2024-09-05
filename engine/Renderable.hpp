#pragma once
#ifndef RENDERABLE_HPP
#define RENDERABLE_HPP

#include <iostream>
#include <vector>
#include <glm/vec3.hpp>
#include <glew.h>
#include <glm/glm.hpp>
#include "fast_obj.h"

#include "IHasUUID.hpp"

class Renderable : public IHasUUID {
private:

	glm::vec3 m_position = { 0.0f, 0.0f, 0.0f };
	glm::vec3 m_rotation = { 0.0f, 0.0f, 0.0f };
	glm::vec3 m_scale = { 1.0f, 1.0f, 1.0f };

	struct Object {
		std::vector<float> m_vecVertices;
		std::vector<unsigned int> m_vecIndices;

		GLuint
			VBO = 0, // Vertex buffer object.
			VAO = 0, // Vertex array object
			EBO = 0; // Element buffer object.

		const fastObjMaterial* m_pMaterial = nullptr;
	};

	std::vector<Renderable::Object*> m_vecObjects;

	glm::mat4 m_matrix;

	const char* m_szUUID;

public:

	Renderable();

	Renderable* bindBuffers(std::string model = std::string("res/models/cube.obj"));

	void renderBuffers();

	Renderable* setPosition(glm::vec3 pos);

	Renderable* setRotation(glm::vec3 rot);

	Renderable* setScale(glm::vec3 scale);

	glm::vec3 getPosition();

	glm::vec3 getRotation();

	glm::vec3 getScale();

	
	const char* getUUID();
};

#endif