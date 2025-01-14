//
// Created by eceva on 1/9/2025.
//

#ifndef GAMEENGINE_CAMERA_H
#define GAMEENGINE_CAMERA_H

#include <iostream>
#include "glm/vec3.hpp"
#include "../utils/patterns/Singleton.h"
#include "../utils/input/MouseHandler.h"
#include "../utils/settings/settings.h"
#include "glm/trigonometric.hpp"
#include "glm/geometric.hpp"

class Camera {
private:
    float xPosition = -1;
    float yPosition = -1;
    float deltaX = -1;
    float deltaY = -1;
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

    void setDeltaX(float deltaX) {
        this->deltaX = deltaX;
    }

    float getDeltaX() const {
        return deltaX;
    }

    void setDeltaY(float deltaY) {
        this->deltaY = deltaY;
    }

    float getDeltaY() const {
        return deltaY;
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

    void update() {
        if(deltaX == 0 && deltaY == 0) return;


        float xOffset = deltaX * Settings::CURSOR_SENSITIVITY;
        float yOffset = deltaY * Settings::CURSOR_SENSITIVITY;

        this->setYaw(yaw + (xOffset));
        this->setPitch(pitch + (yOffset));

        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        // Calculate the new direction vector
        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(direction);
        deltaX = 0;
        deltaY = 0;
    }
};

#endif //GAMEENGINE_CAMERA_H
