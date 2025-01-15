//
// Created by eceva on 1/9/2025.
//
#include <iostream>
#include "CameraHandler.h"
#include "Camera.h"
#include "glm/geometric.hpp"
#include "../settings/settings.h"
#include "glm/trigonometric.hpp"
#include "../input/MouseHandler.h"

void CameraHandler::setCamera(Camera* camera) {
    this->camera = camera;
}

Camera* CameraHandler::getCamera() {
    return this->camera;
}

void CameraHandler::onEvent(const Event &event) {
    return;
}

void CameraHandler::onEvent(const CameraKeyMovementEvent& event) {
    switch (event.direction) {
        case CameraMovementDirection::FORWARD:
            camera->setPosition(
                    camera->getPosition() +
                    (camera->getCalcSpeed(GameState::getInstance().deltaTime) *
                     camera->getFront()));
            break;
        case CameraMovementDirection::BACKWARD:
            camera->setPosition(
                    camera->getPosition() -
                    (camera->getCalcSpeed(GameState::getInstance().deltaTime) * camera->getFront()));
            break;
        case CameraMovementDirection::LEFT:
            camera->setPosition(
                    camera->getPosition() -
                    glm::normalize(glm::cross(camera->getFront(), camera->getUp())) *
                            camera->getCalcSpeed(GameState::getInstance().deltaTime));
            break;
        case CameraMovementDirection::RIGHT:
            camera->setPosition(
                    camera->getPosition() +
                    glm::normalize(glm::cross(camera->getFront(), camera->getUp())) *
                            camera->getCalcSpeed(GameState::getInstance().deltaTime));
            break;
        case CameraMovementDirection::UP:
            camera->setPosition(
                    camera->getPosition() +
                    glm::vec3(0.0f, camera->getCalcSpeed(GameState::getInstance().deltaTime), 0.0f));
            break;
        case CameraMovementDirection::DOWN:
            camera->setPosition(
                    camera->getPosition() -
                    glm::vec3(0.0f, camera->getCalcSpeed(GameState::getInstance().deltaTime), 0.0f));
            break;
    }
}

void CameraHandler::onEvent(const MouseMovementEvent& event) {

}

