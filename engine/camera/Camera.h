//
// Created by eceva on 1/9/2025.
//

#ifndef GAMEENGINE_CAMERA_H
#define GAMEENGINE_CAMERA_H

#include "glm/vec3.hpp"

class Camera {
private:
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
};

#endif //GAMEENGINE_CAMERA_H
