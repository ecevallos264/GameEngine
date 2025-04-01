//
// Created by eceva on 1/10/2025.
//
#include "Camera.h"

void Camera::updatePosition(glm::vec3 updateVector) {
    cameraPos += updateVector;
}

float Camera::getCalcSpeed(float deltaTime) const {
    return cameraSpeed * deltaTime;
}

void Camera::onEvent(const MouseMovementEvent &event) {
    if(MouseHandler::getInstance().getMouseCursorState() == MouseCursorState::OUT_OF_WINDOW) return;
    if(firstRenderingIteration) {
        firstRenderingIteration = false;
        return;
    }
    if(event.deltaX == 0 && event.deltaY == 0) return;


    float xOffset = event.deltaX * Settings::CURSOR_SENSITIVITY;
    float yOffset = event.deltaY * Settings::CURSOR_SENSITIVITY;

    this->setYaw(yaw + (xOffset));
    this->setPitch(pitch + (yOffset));

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);

    glm::vec3 worldUp(0.0f, 1.0f, 0.0f);

    glm::vec3 cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
    cameraUp    = glm::normalize(glm::cross(cameraRight, cameraFront));
}

void Camera::onEvent(const Event &event) {

}
