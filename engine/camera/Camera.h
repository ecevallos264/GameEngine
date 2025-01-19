//
// Created by eceva on 1/9/2025.
//

#ifndef GAMEENGINE_CAMERA_H
#define GAMEENGINE_CAMERA_H

#include <iostream>
#include "glm/vec3.hpp"
#include "../core/patterns/Singleton.h"
#include "../input/MouseHandler.h"
#include "../core/settings/settings.h"
#include "glm/trigonometric.hpp"
#include "glm/geometric.hpp"
#include "../core/eventing/events/MouseMovementEvent.h"
#include "../core/eventing/EventDispatcher.h"
#include "glm/fwd.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"

class Camera : EventListener {
private:
    bool firstRenderingIteration = true;
    
    float xPosition;
    float yPosition;
    float yaw = -90.0f;
    float pitch;
    float roll;

    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
    glm::vec3 direction;

    float cameraSpeed;

public:
    Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up) : cameraPos(position), cameraFront(front), cameraUp(up) {
        EventDispatcher::getInstance().registerListener<MouseMovementEvent>([this](const Event& event) {
            this->onEvent(dynamic_cast<const MouseMovementEvent&>(event));
        });
    }

    void updatePosition(glm::vec3 updateVector);

    glm::vec3 getPosition();
    glm::vec3 getFront();
    glm::vec3 getUp();
    void setPosition(glm::vec3 vector);
    void setFront(glm::vec3 vector);
    void setUp(glm::vec3 vector);
    void setSpeed(float speed);

    float getCalcSpeed(float deltaTime) const;

    void setXPosition(float xPosition) {
        this->xPosition = xPosition;
    }

    float getXPosition() const {
        return xPosition;
    }

    void setYPosition(float yPosition) {
        this->yPosition = yPosition;
    }

    float getYPosition() const {
        return yPosition;
    }

    glm::mat4 getViewMatrix() {
        return glm::lookAt(
                cameraPos,
                cameraPos + cameraFront,
                cameraUp
        );
    }

    glm::mat4 getProjectionMatrix() {
        return glm::perspective(glm::radians(45.0f), (float)Settings::WINDOW_WIDTH / (float)Settings::WINDOW_HEIGHT, 0.1f, Settings::MAX_RENDER_DISTANCE);
    }

    void setYaw(float yaw) {
        this->yaw = yaw;
    }

    float getYaw() const {
        return yaw;
    }

    void setPitch(float pitch) {
        this->pitch = pitch;
    }

    float getPitch() const {
        return pitch;
    }

    void setRoll(float roll) {
        this->roll = roll;
    }

    float getRoll() const {
        return roll;
    }

    void onEvent(const MouseMovementEvent& event);
    void onEvent(const Event& event) override;
};

#endif //GAMEENGINE_CAMERA_H
