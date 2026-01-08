#ifndef GAMEENGINE_ECS_CAMERA_COMPONENT_H
#define GAMEENGINE_ECS_CAMERA_COMPONENT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace ECS {

enum class CameraMode {
    FPS,        // First-person: yaw/pitch mouse look, WASD movement
    Orbit,      // Third-person: orbits around target point
    Fixed       // Static camera: uses TransformComponent rotation directly
};

enum class CameraProjection {
    Perspective,
    Orthographic
};

enum class CameraTag {
    None = 0,
    Main = 1,       // Primary rendering camera
    Debug = 2,      // Debug/fly-through camera
    UI = 3,         // UI overlay camera
    Shadow = 4      // Shadow mapping camera
};

struct CameraComponent {
    // Projection parameters
    float fov = 45.0f;                  // Field of view in degrees (perspective)
    float nearPlane = 0.1f;
    float farPlane = 20000.0f;
    float aspectRatio = 16.0f / 9.0f;
    float orthoSize = 10.0f;            // Half-height of ortho view
    CameraProjection projection = CameraProjection::Perspective;

    // Mode and state
    CameraMode mode = CameraMode::FPS;
    CameraTag tag = CameraTag::None;
    int priority = 0;                   // Higher = more priority for selection
    bool active = true;

    // FPS mode: yaw/pitch for mouse look
    float yaw = -90.0f;
    float pitch = 0.0f;
    float sensitivity = 0.1f;
    float moveSpeed = 5.0f;
    bool constrainPitch = true;
    float maxPitch = 89.0f;

    // Orbit mode: target + distance
    glm::vec3 orbitTarget = glm::vec3(0.0f);
    float orbitDistance = 10.0f;
    float orbitMinDistance = 1.0f;
    float orbitMaxDistance = 100.0f;

    // Cached matrices (computed by CameraSystem)
    glm::mat4 viewMatrix = glm::mat4(1.0f);
    glm::mat4 projectionMatrix = glm::mat4(1.0f);
    glm::mat4 viewProjectionMatrix = glm::mat4(1.0f);

    // Dirty flag for optimization
    bool dirty = true;

    CameraComponent() = default;

    CameraComponent(CameraTag cameraTag, int cameraPriority = 0)
        : tag(cameraTag), priority(cameraPriority) {}

    CameraComponent(float fieldOfView, float nearClip, float farClip)
        : fov(fieldOfView), nearPlane(nearClip), farPlane(farClip) {}

    // Get front vector based on yaw/pitch (FPS mode)
    glm::vec3 getFront() const {
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        return glm::normalize(front);
    }

    // Get right vector
    glm::vec3 getRight() const {
        return glm::normalize(glm::cross(getFront(), glm::vec3(0.0f, 1.0f, 0.0f)));
    }

    // Get up vector
    glm::vec3 getUp() const {
        return glm::normalize(glm::cross(getRight(), getFront()));
    }

    // Apply mouse movement (FPS mode)
    void applyMouseDelta(float xOffset, float yOffset) {
        yaw += xOffset * sensitivity;
        pitch += yOffset * sensitivity;

        if (constrainPitch) {
            if (pitch > maxPitch) pitch = maxPitch;
            if (pitch < -maxPitch) pitch = -maxPitch;
        }

        dirty = true;
    }

    // Adjust orbit distance (Orbit mode)
    void adjustOrbitDistance(float delta) {
        orbitDistance -= delta;
        if (orbitDistance < orbitMinDistance) orbitDistance = orbitMinDistance;
        if (orbitDistance > orbitMaxDistance) orbitDistance = orbitMaxDistance;
        dirty = true;
    }
};

} // namespace ECS

#endif //GAMEENGINE_ECS_CAMERA_COMPONENT_H
