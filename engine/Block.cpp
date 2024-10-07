#include "Block.hpp"

Block::Block(glm::vec3 position, glm::vec3 rotation) {
	this->m_vecPosition = position;
	this->m_vecRotation = rotation;
}

Block::~Block() {}