//
// Created by eceva on 1/9/2025.
//

#ifndef GAMEENGINE_CAMERA_H
#define GAMEENGINE_CAMERA_H

#include "glm/vec3.hpp"
#include "../utils/patterns/Singleton.h"
#include "../utils/input/MouseHandler.h"
#include "../utils/settings/settings.h"
#include "glm/trigonometric.hpp"
#include "glm/geometric.hpp"

class Camera {
private:
    float xPosition;
    float yPosition;
    float lastXPosition = -1;
    float lastYPosition = -1;
    float yaw = -90.0f;
    float pitch;
    float roll;

    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
    glm::vec3 direction;

    float cameraSpeed;

public:
    Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up) : cameraPos(position), cameraFront(front), cameraUp(up) {}

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
        this->lastXPosition = xPosition;
    }

    float getXPosition() const {
        return lastXPosition;
    }

    void setYPosition(float yPosition) {
        this->lastYPosition = yPosition;
    }

    float getYPosition() const {
        return lastYPosition;
    }

    void setLastXPosition(float lastXPosition) {
        this->lastXPosition = lastXPosition;
    }

    float getLastXPosition() const {
        return lastXPosition;
    }

    void setLastYPosition(float lastYPosition) {
        this->lastYPosition = lastYPosition;
    }

    float getLastYPosition() const {
        return lastYPosition;
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

    void update(float deltaX, float deltaY) {
        if (MouseHandler::getInstance().getMouseCursorState() != MouseCursorState::IN_WINDOW)
            return;
//        if(event.prevXPosition == -1 || event.prevYPosition == -1) {
//            this->setLastXPosition(event.xPosition);
//            this->setLastYPosition(event.yPosition);
//            return;
//        }

//        float xOffset = (event.xPosition - event.prevXPosition) * Settings::CURSOR_SENSITIVITY;
//        float yOffset = (event.prevYPosition - event.yPosition) * Settings::CURSOR_SENSITIVITY;

//        this->setLastXPosition(event.xPosition);
//        this->setLastYPosition(event.yPosition);

        this->setYaw(this->getYaw() + (deltaX * Settings::CURSOR_SENSITIVITY));
        this->setPitch(this->getPitch() + (deltaX * Settings::CURSOR_SENSITIVITY));

        // Clamp the pitch to avoid gimbal lock
        if (this->getPitch() > 89.0f)
            this->setPitch(89.0f);
        if (this->getPitch() < -89.0f)
            this->setPitch(-89.0f);

        // Calculate the new direction vector
        glm::vec3 direction;
        direction.x = cos(glm::radians(this->getYaw())) * cos(glm::radians(this->getPitch()));
        direction.y = sin(glm::radians(this->getPitch()));
        direction.z = sin(glm::radians(this->getYaw())) * cos(glm::radians(this->getPitch()));
        this->setFront(glm::normalize(direction));
    }
};

#endif //GAMEENGINE_CAMERA_H
