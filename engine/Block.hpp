#pragma once

#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <glm/vec3.hpp>

class Block {
private:

public:

	glm::vec3 m_vecPosition;
	glm::vec3 m_vecRotation;

	Block(glm::vec3 position, glm::vec3 rotation);
	~Block();
};


#endif // !BLOCK_HPP