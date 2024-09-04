#include "Renderable.hpp"
#include "Engine.hpp"
#include "fast_obj.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

Renderable::Renderable() {
    this->m_matrix = glm::mat4(1.f);
    this->m_szUUID = "hi";
}

Renderable* Renderable::bindBuffers(std::string model) {
    fastObjMesh* mesh = fast_obj_read(model.c_str());

    unsigned int offset = 0;  // Cumulative offset

    for (unsigned int i = 0; i < mesh->face_count; i++) {
        unsigned int face_vertex_count = mesh->face_vertices[i]; // Number of vertices in this face
        for (unsigned int j = 0; j < face_vertex_count; j++) {
            unsigned int index = mesh->indices[offset + j].p;
            this->m_indices.push_back(this->m_indices.size());

            // Add vertex positions
            this->m_vertices.push_back(mesh->positions[index * 3 + 0]);
            this->m_vertices.push_back(mesh->positions[index * 3 + 1]);
            this->m_vertices.push_back(mesh->positions[index * 3 + 2]);

            // Add texture coordinates (if available)
            if (mesh->indices[offset + j].t != -1) {
                this->m_vertices.push_back(mesh->texcoords[mesh->indices[offset + j].t * 2 + 0]);
                this->m_vertices.push_back(mesh->texcoords[mesh->indices[offset + j].t * 2 + 1]);
            }

            // Add normals (if available)
            if (mesh->indices[offset + j].n != -1) {
                this->m_vertices.push_back(mesh->normals[mesh->indices[offset + j].n * 3 + 0]);
                this->m_vertices.push_back(mesh->normals[mesh->indices[offset + j].n * 3 + 1]);
                this->m_vertices.push_back(mesh->normals[mesh->indices[offset + j].n * 3 + 2]);
            }
        }

        // Increment offset by the number of vertices in the current face
        offset += face_vertex_count;
    }

    // Bind the VAO before setting up vertex attributes and buffers
    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);

    // Upload vertex data to VBO
    glGenBuffers(1, &this->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, this->m_vertices.size() * sizeof(float), this->m_vertices.data(), GL_STATIC_DRAW);

    // Upload index data to EBO (after binding VAO so it gets stored in the VAO)
    glGenBuffers(1, &this->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->m_indices.size() * sizeof(unsigned int), this->m_indices.data(), GL_STATIC_DRAW);

    // Specify vertex attributes
    // Assuming positions (3 floats), texcoords (2 floats), and normals (3 floats)
    GLsizei stride = (3 + 2 + 3) * sizeof(float); // 3 for position, 2 for texcoords, 3 for normals
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);  // Position attribute
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));  // Texcoords attribute
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));  // Normals attribute
    glEnableVertexAttribArray(2);

    // Unbind the VBO and VAO (EBO remains bound to VAO)
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return this;
}

void Renderable::renderBuffers() {
    this->m_matrix = glm::translate(glm::mat4(1.f), this->m_position);
    //this->m_matrix = glm::rotate(this->m_matrix, glm::radians(45.f), this->m_rotation);

    GLuint iUniformLocation = glGetUniformLocation(RENDER()->getShaderProgram(), "m_mat4Model");
    glUniformMatrix4fv(iUniformLocation, 1, GL_FALSE, glm::value_ptr(this->m_matrix));

    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, this->m_indices.size(), GL_UNSIGNED_INT, 0);
}

Renderable* Renderable::setPosition(glm::vec3 pos) {
    this->m_position = pos;
    return this;
}

Renderable* Renderable::setRotation(glm::vec3 rot) {
    this->m_rotation = rot;
    return this;
}

glm::vec3 Renderable::getPosition() {
    return this->m_position;
}

glm::vec3 Renderable::getRotation() {
    return this->m_rotation;
}

const char* Renderable::getUUID() {
    return (const char*)this->m_szUUID;
}