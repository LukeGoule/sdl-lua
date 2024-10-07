#pragma once

#ifndef CHUNK_HPP
#define CHUNK_HPP

#include <string>
#include <vector>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glew.h>

// X width.
#define CHUNK_X_SIZE 16

// Y height (up-down).
#define CHUNK_Y_SIZE 64

// Z width.
#define CHUNK_Z_SIZE 16

#define BLOCK_Y_POS 0
#define BLOCK_Y_NEG 1
#define BLOCK_X_POS 2
#define BLOCK_X_NEG 3
#define BLOCK_Z_POS 4
#define BLOCK_Z_NEG 5

class Block;

class Chunk {

	/*
	 * Array of blocks. A nullptr inside this array indicates an air block.
	 */
	Block** m_vecBlocks = nullptr;

	typedef struct BlockMesh {
		long index;

		std::string model;

		void* m_pFastObjMesh = nullptr;
		std::vector<float> m_vecVertices;
		std::vector<float> m_vecIndices;
	};

	/*
	 * Array of block meshes.
	 */
	void** m_vecMeshes = nullptr;

	/*
	 * Array of block vertices.
	 */
	std::vector<float> m_vecVertices;

	std::vector<float> m_vecIndices;

	GLuint VAO, VBO, EBO;

	glm::mat4 m_matrix;

	glm::vec3 m_position = glm::vec3{ 0,0,0 };

	void* m_pMaterial;

public:

	Chunk();
	~Chunk();

	void generateMesh();

	void pushMesh(int index, int x, int y, int z);

	void renderBuffers();

	Chunk* setBlock(size_t index, Block* pBlock);

	Block* getBlock(size_t index);

	static size_t getIndexAt(int x, int y, int z);
};

#endif // !CHUNK_HPP