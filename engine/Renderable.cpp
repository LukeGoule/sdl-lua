#include "Renderable.hpp"
#include "Engine.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

Renderable::Renderable(const char* szModel) {
    this->m_matrix = glm::mat4(1.f);
    this->m_szUUID = "hi";

    if (szModel) {
        this->setModel(szModel);
    }
}

Renderable* Renderable::bindBuffers() {
    if (!this->m_pMesh) {
        throw new std::exception("Mesh not set when attempting to bind buffer objects.");
    }
    
    for (size_t i = 0; i < this->m_pMesh->object_count; i++) {
        const auto object = &this->m_pMesh->objects[i];
        const auto material = (fastObjMaterial*) (&this->m_pMesh->materials[i]);

        if (object->name) {
            std::cout << "\t- Loading object: " << object->name << std::endl;
        }

        unsigned int index_offset = object->index_offset;
        const auto face_offset = object->face_offset;
        
        const auto newObject = new Renderable::Object{};
        
        newObject->m_pMaterial = material ? material : new fastObjMaterial{
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
        };

        if (newObject->m_pMaterial->name) {
            std::cout << "\t\t- Material: " << newObject->m_pMaterial->name << std::endl;
        }

        for (unsigned int i = 0; i < object->face_count; i++) {
            unsigned int face_vertex_count = this->m_pMesh->face_vertices[face_offset + i]; // Use face_offset for correct indexing

            for (unsigned int j = 0; j < face_vertex_count; j++) {
                const auto indexInfo = &this->m_pMesh->indices[index_offset + j];
                unsigned int index = indexInfo->p;
                newObject->m_vecIndices.push_back(newObject->m_vecIndices.size());

                // Add vertex positions
                newObject->m_vecVertices.push_back(this->m_pMesh->positions[index * 3 + 0] * this->m_scale.x);
                newObject->m_vecVertices.push_back(this->m_pMesh->positions[index * 3 + 1] * this->m_scale.x);
                newObject->m_vecVertices.push_back(this->m_pMesh->positions[index * 3 + 2] * this->m_scale.x);

                newObject->m_vecVertices.push_back(1.f);
                newObject->m_vecVertices.push_back(1.f);
                newObject->m_vecVertices.push_back(1.f);

                // Add texture coordinates (if available)
                if (indexInfo->t != -1) {
                    newObject->m_vecVertices.push_back(this->m_pMesh->texcoords[indexInfo->t * 2 + 0]);
                    newObject->m_vecVertices.push_back(this->m_pMesh->texcoords[indexInfo->t * 2 + 1]);
                }

                // Add normals (if available)
                if (indexInfo->n != -1) {
                    newObject->m_vecVertices.push_back(this->m_pMesh->normals[indexInfo->n * 3 + 0]);
                    newObject->m_vecVertices.push_back(this->m_pMesh->normals[indexInfo->n * 3 + 1]);
                    newObject->m_vecVertices.push_back(this->m_pMesh->normals[indexInfo->n * 3 + 2]);
                }
            }

            index_offset += face_vertex_count;
        }

        // Bind the VAO before setting up vertex attributes and buffers
        glGenVertexArrays(1, &newObject->VAO);
        glBindVertexArray(newObject->VAO);

        // Upload vertex data to VBO
        glGenBuffers(1, &newObject->VBO);
        glBindBuffer(GL_ARRAY_BUFFER, newObject->VBO);
        glBufferData(GL_ARRAY_BUFFER, newObject->m_vecVertices.size() * sizeof(float), newObject->m_vecVertices.data(), GL_STATIC_DRAW);

        // Upload index data to EBO (after binding VAO so it gets stored in the VAO)
        glGenBuffers(1, &newObject->EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newObject->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, newObject->m_vecIndices.size() * sizeof(unsigned int), newObject->m_vecIndices.data(), GL_STATIC_DRAW);

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

        this->m_vecObjects.push_back(newObject);
    }

    return this;
}

void Renderable::renderBuffers() {
    this->m_matrix = glm::translate(glm::mat4(1.f), this->m_position);
    this->m_matrix = glm::rotate(this->m_matrix, glm::radians(this->m_rotation.x), glm::vec3(1.f,0.f,0.f));
    this->m_matrix = glm::rotate(this->m_matrix, glm::radians(this->m_rotation.y), glm::vec3(0.f, 1.f, 0.f));
    this->m_matrix = glm::rotate(this->m_matrix, glm::radians(this->m_rotation.z), glm::vec3(0.f, 0.f, 1.f));
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

    for (const auto pObject : this->m_vecObjects) {
        glUniform3fv(m_vec3Ambient, 1, pObject->m_pMaterial->Ka);
        glUniform3fv(m_vec3Diffuse, 1, pObject->m_pMaterial->Kd);
        glUniform3fv(m_vec3Specular, 1, pObject->m_pMaterial->Ks);
        glUniform3fv(m_vec3Emission, 1, pObject->m_pMaterial->Ke);
        glUniform1fv(m_fShininess, 1, &pObject->m_pMaterial->Ns);
        glUniform1fv(m_fRefractoryIndex, 1, &pObject->m_pMaterial->Ni);
        glUniform1fv(m_fDisolve, 1, &pObject->m_pMaterial->d);
        glUniform1iv(m_iIllumination, 1, &pObject->m_pMaterial->illum);

        glBindVertexArray(pObject->VAO);
        glDrawElements(GL_TRIANGLES, pObject->m_vecIndices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}


Renderable* Renderable::setPosition(glm::vec3 pos) {
    this->m_position = pos;
    return this;
}

Renderable* Renderable::setRotation(glm::vec3 rot) {
    this->m_rotation = rot;
    return this;
}

Renderable* Renderable::setScale(glm::vec3 scale) {
    this->m_scale = scale;
    return this;
}

Renderable* Renderable::setModel(fastObjMesh* pMesh) {
    this->m_pMesh = pMesh;
    return this;
}

Renderable* Renderable::setModel(const char* szModelName) {
    this->m_pMesh = RENDER()->getModel(szModelName);
    return this;
}


glm::vec3 Renderable::getPosition() {
    return this->m_position;
}

glm::vec3 Renderable::getRotation() {
    return this->m_rotation;
}

glm::vec3 Renderable::getScale() {
    return this->m_scale;
}


const char* Renderable::getUUID() {
    return (const char*)this->m_szUUID;
}