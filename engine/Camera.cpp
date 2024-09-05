#include "Camera.hpp"
#include "Keyboard.hpp"
#include "Engine.hpp"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

Camera::Camera(Engine* instance) : EngineModule(instance) {
    this->position = glm::vec3(0.0f, 0.0f, 3.0f);
    this->front = glm::vec3(0.0f, 0.0f, -1.0f);
    this->up = glm::vec3(0.0f, 1.0f, 0.0f);
    this->yaw =-90.0f;
    this->pitch = 0.0f;
    this->speed = 10.f;
    this->sensitivity = 0.1f;
}

void Camera::handleSDLEvent(SDL_Event* pEvent) {
    if (pEvent->type == SDL_MOUSEMOTION) {
        this->processMouseMovement(pEvent->motion.xrel, pEvent->motion.yrel);
    }
}

void Camera::processMouseMovement(float xOffset, float yOffset) {
    if (this->locked) {
        return;
    }

    xOffset *= this->sensitivity;
    yOffset *= -this->sensitivity;

    this->yaw += xOffset;
    this->pitch += yOffset;

    // Constrain the pitch
    if (this->pitch > 89.0f)
        this->pitch = 89.0f;
    if (this->pitch < -89.0f)
        this->pitch = -89.0f;

    // Update front vector
    glm::vec3 front;
    front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    front.y = sin(glm::radians(this->pitch));
    front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    this->front = glm::normalize(front);
}

void Camera::processKeyboard(float deltaTime) {
    float velocity = this->speed * deltaTime;

    const auto pKeyboard = KEYBOARD();

    if (!this->locked) {

        if (pKeyboard->checkState(SDLK_w)) {
            this->position += this->front * velocity;
        }

        if (pKeyboard->checkState(SDLK_s)) {
            this->position -= this->front * velocity;
        }

        if (pKeyboard->checkState(SDLK_a)) {
            this->position -= glm::normalize(glm::cross(this->front, this->up)) * velocity;
        }

        if (pKeyboard->checkState(SDLK_d)) {
            this->position += glm::normalize(glm::cross(this->front, this->up)) * velocity;
        }

        if (pKeyboard->checkState(SDLK_LSHIFT)) {
            this->speed = 10.f;
        }
        else {
            this->speed = 2.5f;
        }
    }

    if (KEYBOARD()->checkState(SDLK_F5)) {
        SDL_SetRelativeMouseMode(SDL_TRUE);
        this->locked = false;
    }
    else if (KEYBOARD()->checkState(SDLK_F6)) {
        SDL_SetRelativeMouseMode(SDL_FALSE);
        this->locked = true;
    }
}

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(this->position, this->position + this->front, this->up);
}

float* Camera::getPositionPtr() {
    return static_cast<float*>(glm::value_ptr<float,glm::packed_highp>(this->position));
}