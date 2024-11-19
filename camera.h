// camera.h
#ifndef SOFTWAREENGINEERINGPROJECT_CAMERA_H
#define SOFTWAREENGINEERINGPROJECT_CAMERA_H

#include <glm/glm.hpp>

class Camera {
public:
    Camera(const Camera&) = delete;
    Camera& operator=(const Camera&) = delete;

    static Camera& getInstance() {
        static Camera instance;
        return instance;
    }

    void setPosition(const glm::vec3& position) {
        cameraPos = position;
    }

    glm::vec3 getPosition() const {
        return cameraPos;
    }

    void setFront(const glm::vec3& front) {
        cameraFront = front;
    }

    glm::vec3 getFront() const {
        return cameraFront;
    }

    void setUp(const glm::vec3& up) {
        cameraUp = up;
    }

    glm::vec3 getUp() const {
        return cameraUp;
    }

    float getSpeed(float deltaTime) const {
        return cameraSpeed * deltaTime;
    }

    void setSpeed(float speed) {
        cameraSpeed = speed;
    }

private:
    Camera() : cameraPos(0.0f, 0.0f, 3.0f), cameraFront(0.0f, 0.0f, -1.0f), cameraUp(0.0f, 1.0f, 0.0f) {}

    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
    float cameraSpeed;
};

#endif // SOFTWAREENGINEERINGPROJECT_CAMERA_H