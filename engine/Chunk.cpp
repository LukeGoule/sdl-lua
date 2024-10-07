#include "Chunk.hpp"
#include "fast_obj.h"
#include "Block.hpp"
#include "Engine.hpp"
#include "Renderable.hpp"

Chunk::Chunk() {
	// Allocate the memory for this chunks block pointers.
	this->m_vecBlocks = new Block* [CHUNK_X_SIZE * CHUNK_Y_SIZE * CHUNK_Z_SIZE];

	//// Allocate the mesh pointers array.
	this->m_vecMeshes = new void* [6];

	this->m_vecMeshes[BLOCK_Y_POS] = (void*)fast_obj_read("res/models/cube/Y-POS.obj");
	this->m_vecMeshes[BLOCK_Y_NEG] = (void*)fast_obj_read("res/models/cube/Y-NEG.obj");
	this->m_vecMeshes[BLOCK_X_POS] = (void*)fast_obj_read("res/models/cube/X-POS.obj");
	this->m_vecMeshes[BLOCK_X_NEG] = (void*)fast_obj_read("res/models/cube/X-NEG.obj");
	this->m_vecMeshes[BLOCK_Z_POS] = (void*)fast_obj_read("res/models/cube/Z-POS.obj");
	this->m_vecMeshes[BLOCK_Z_NEG] = (void*)fast_obj_read("res/models/cube/Z-NEG.obj");

    this->m_vecVertices = std::vector<float>();
    this->m_vecIndices = std::vector<float>();
}

Chunk::~Chunk() {

}

void Chunk::generateMesh() {
    std::cout << "setting up chunk mesh" << std::endl;
	for (int z = 0; z < CHUNK_Z_SIZE; z++) {
		for (int y = 0; y < CHUNK_Y_SIZE; y++) {
			for (int x = 0; x < CHUNK_X_SIZE; x++) {
                this->pushMesh(BLOCK_Y_POS, x, y, z);
			}
		}
	}
    std::cout << "done!" << std::endl;
}

void Chunk::pushMesh(int index, int x, int y, int z) {
    const auto pMesh = static_cast<fastObjMesh*>(this->m_vecMeshes[index]);

    for (size_t i = 0; i < pMesh->object_count; i++) {
        const auto object = &pMesh->objects[i];
        const auto material = (fastObjMaterial*)(&pMesh->materials[i]);

        if (object->name) {
            //std::cout << "\t- Loading object: " << object->name << std::endl;
        }

        unsigned int index_offset = object->index_offset;
        const auto face_offset = object->face_offset;

        
        this->m_pMaterial = material ? (void*)material : (void*)(new fastObjMaterial{
            (char*)"Empty Material",
            {1.f,1.f,1.f},
            {1.f,1.f,1.f},
            {1.f,1.f,1.f},
            {1.f,1.f,1.f},
            {1.f,1.f,1.f},
            1.f,
            1.f,
            {1.f,1.f,1.f},
            1.f,
            1
        });

        const auto scale = 1.f;


        for (unsigned int i = 0; i < object->face_count; i++) {
            unsigned int face_vertex_count = pMesh->face_vertices[face_offset + i]; // Use face_offset for correct indexing

            for (unsigned int j = 0; j < face_vertex_count; j++) {
                const auto indexInfo = &pMesh->indices[index_offset + j];
                unsigned int index = indexInfo->p;
                this->m_vecIndices.push_back(this->m_vecIndices.size());

                // Add vertex positions
                this->m_vecVertices.push_back(pMesh->positions[index * 3 + 0] * scale);
                this->m_vecVertices.push_back(pMesh->positions[index * 3 + 1] * scale);
                this->m_vecVertices.push_back(pMesh->positions[index * 3 + 2] * scale);

                this->m_vecVertices.push_back(1.f);
                this->m_vecVertices.push_back(1.f);
                this->m_vecVertices.push_back(1.f);

                // Add texture coordinates (if available)
                if (indexInfo->t != -1) {
                    this->m_vecVertices.push_back(pMesh->texcoords[indexInfo->t * 2 + 0]);
                    this->m_vecVertices.push_back(pMesh->texcoords[indexInfo->t * 2 + 1]);
                }

                // Add normals (if available)
                if (indexInfo->n != -1) {
                    this->m_vecVertices.push_back(pMesh->normals[indexInfo->n * 3 + 0]);
                    this->m_vecVertices.push_back(pMesh->normals[indexInfo->n * 3 + 1]);
                    this->m_vecVertices.push_back(pMesh->normals[indexInfo->n * 3 + 2]);
                }
            }

            index_offset += face_vertex_count;
        }

        // Bind the VAO before setting up vertex attributes and buffers
        glGenVertexArrays(1, &this->VAO);
        glBindVertexArray(this->VAO);

        // Upload vertex data to VBO
        glGenBuffers(1, &this->VBO);
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferData(GL_ARRAY_BUFFER, this->m_vecVertices.size() * sizeof(float), this->m_vecVertices.data(), GL_STATIC_DRAW);

        // Upload index data to EBO (after binding VAO so it gets stored in the VAO)
        glGenBuffers(1, &this->EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->m_vecIndices.size() * sizeof(unsigned int), this->m_vecIndices.data(), GL_STATIC_DRAW);

        // Specify vertex attributes
        // Assuming positions (3 floats), colours (3 floats), texcoords (2 floats), and normals (3 floats)
        GLsizei stride = (3 + 3 + 2 + 3) * sizeof(float); // 3 for position, 3 for colours, 2 for texcoords, 3 for normals
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);  // Position attribute
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));  // Colours attribute
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));  // Texcoords attribute
        glEnableVertexAttribArray(2);

        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride, (void*)(8 * sizeof(float)));  // Normals attribute
        glEnableVertexAttribArray(3);

        // Unbind the VBO and VAO (EBO remains bound to VAO)
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
}

void Chunk::renderBuffers() {
    this->m_matrix = glm::translate(glm::mat4(1.f), this->m_position);
    //this->m_matrix = glm::rotate(this->m_matrix, glm::radians(this->m_rotation.x), glm::vec3(1.f, 0.f, 0.f));
    //this->m_matrix = glm::rotate(this->m_matrix, glm::radians(this->m_rotation.y), glm::vec3(0.f, 1.f, 0.f));
    //this->m_matrix = glm::rotate(this->m_matrix, glm::radians(this->m_rotation.z), glm::vec3(0.f, 0.f, 1.f));
    //this->m_matrix = glm::scale(this->m_matrix, this->m_scale);

    const auto iShaderProgram = RENDER()->getShaderProgram();

    GLuint m_mat4Model = glGetUniformLocation(iShaderProgram, "m_mat4Model");
    GLuint m_vec3Ambient = glGetUniformLocation(iShaderProgram, "m_vec3Ambient");
    GLuint m_vec3Diffuse = glGetUniformLocation(iShaderProgram, "m_vec3Diffuse");
    GLuint m_vec3Specular = glGetUniformLocation(iShaderProgram, "m_vec3Specular");
    GLuint m_vec3Emission = glGetUniformLocation(iShaderProgram, "m_vec3Emission");
    GLuint m_fShininess = glGetUniformLocation(iShaderProgram, "m_fShininess");
    GLuint m_fRefractoryIndex = glGetUniformLocation(iShaderProgram, "m_fRefractoryIndex");
    GLuint m_fDisolve = glGetUniformLocation(iShaderProgram, "m_fDisolve");
    GLuint m_iIllumination = glGetUniformLocation(iShaderProgram, "m_iIllumination");

    glUniformMatrix4fv(m_mat4Model, 1, GL_FALSE, glm::value_ptr(this->m_matrix));

    const fastObjMaterial* pMaterial = (const fastObjMaterial*)this->m_pMaterial;

    glUniform3fv(m_vec3Ambient, 1, pMaterial->Ka);
    glUniform3fv(m_vec3Diffuse, 1, pMaterial->Kd);
    glUniform3fv(m_vec3Specular, 1, pMaterial->Ks);
    glUniform3fv(m_vec3Emission, 1, pMaterial->Ke);
    glUniform1fv(m_fShininess, 1, &pMaterial->Ns);
    glUniform1fv(m_fRefractoryIndex, 1, &pMaterial->Ni);
    glUniform1fv(m_fDisolve, 1, &pMaterial->d);
    glUniform1iv(m_iIllumination, 1, &pMaterial->illum);

    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, this->m_vecIndices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}


Chunk* Chunk::setBlock(size_t index, Block* pBlock) {
	this->m_vecBlocks[index] = pBlock;
	return this;
}

Block* Chunk::getBlock(size_t index) {
	return this->m_vecBlocks[index];
}

size_t Chunk::getIndexAt(int x, int y, int z) {
	return x + (y * CHUNK_X_SIZE) + (z * CHUNK_X_SIZE * CHUNK_Y_SIZE);
}