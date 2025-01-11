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

glm::vec3 Camera::getPosition() {
    return cameraPos;
}

glm::vec3 Camera::getFront() {
    return cameraFront;
}

glm::vec3 Camera::getUp() {
    return cameraUp;
}

void Camera::setPosition(glm::vec3 vector) {
    cameraPos = vector;
}

void Camera::setFront(glm::vec3 vector) {
    this->cameraFront = vector;
}

void Camera::setUp(glm::vec3 vector) {
    this->cameraUp = vector;
}

void Camera::setSpeed(float speed) {
    this->cameraSpeed = speed;
}