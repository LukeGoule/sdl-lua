#pragma once

#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "EngineModule.hpp"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "SDL.h"

/*
 *
 * Our keyboard controller class. This handles the states of the keys on your keyboard.
 *
 */
class Camera : public EngineModule {
public:
    Camera(Engine* instance);

    /*
     * Handle SDL events.
     */
    void handleSDLEvent(SDL_Event* pEvent);

    /*
     * Get the view matrix for the shader.
     */
    glm::mat4 getViewMatrix();

    /*
     * Get pointer to the position.
     */
    float* getPositionPtr();

    /*
     * Process keyboard inputs.
     */
    void processKeyboard(float deltaTime);

private:

    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    float yaw;
    float pitch;
    float speed;
    float sensitivity;
    bool locked = true;

    /*
     * Process mouse movement delta.
     */
    void processMouseMovement(float xOffset, float yOffset);
};

#endif // !CAMERA_HPP