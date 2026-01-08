#ifndef GAMEENGINE_ECS_CAMERA_INPUT_SYSTEM_H
#define GAMEENGINE_ECS_CAMERA_INPUT_SYSTEM_H

#include "../../runtime/ISystem.h"
#include "../Registry.h"
#include "../components/CameraComponent.h"
#include "../components/TransformComponent.h"
#include "CameraSystem.h"
#include "../../eventing/EventDispatcher.h"
#include "../../eventing/events/CameraKeyMovementEvent.h"
#include "../../eventing/events/MouseMovementEvent.h"
#include "../events/CameraFocusEvent.h"

namespace ECS {

// Tag component to mark entity as controllable by input
struct CameraControllerComponent {
    bool mouseControlEnabled = true;
    bool keyboardControlEnabled = true;
    bool firstMouse = true;
    bool focused = false;  // Start unfocused for editor mode (ESC to toggle)

    CameraControllerComponent() = default;
};

class CameraInputSystem : public ISystem {
public:
    CameraInputSystem(Registry& registry, CameraSystem& cameraSystem)
        : registry(registry), cameraSystem(cameraSystem) {}

    void initialize(SystemContext& context) override {
        (void)context;

        // Register for keyboard movement events
        EventDispatcher::getInstance().registerListener<CameraKeyMovementEvent>(
            [this](const Event& event) {
                this->onCameraKeyMovement(dynamic_cast<const CameraKeyMovementEvent&>(event));
            }
        );

        // Register for mouse movement events
        EventDispatcher::getInstance().registerListener<MouseMovementEvent>(
            [this](const Event& event) {
                this->onMouseMovement(dynamic_cast<const MouseMovementEvent&>(event));
            }
        );

        // Register for camera focus events
        EventDispatcher::getInstance().registerListener<CameraFocusEvent>(
            [this](const Event& event) {
                this->onCameraFocusChanged(dynamic_cast<const CameraFocusEvent&>(event));
            }
        );
    }

private:
    Registry& registry;
    CameraSystem& cameraSystem;

    void onCameraKeyMovement(const CameraKeyMovementEvent& event) {
        // Process movement for all controllable cameras
        registry.view<TransformComponent, CameraComponent, CameraControllerComponent>().each(
            [&](Entity entity, TransformComponent& transform,
                CameraComponent& camera, CameraControllerComponent& controller) {
                (void)entity;

                if (!controller.focused) return;
                if (!controller.keyboardControlEnabled) return;
                if (!camera.active) return;

                processMovement(transform, camera, event.direction, static_cast<float>(event.deltaTime));
            }
        );
    }

    void onCameraFocusChanged(const CameraFocusEvent& event) {
        // Update focus state for all controllable cameras
        registry.view<CameraControllerComponent>().each(
            [&](Entity entity, CameraControllerComponent& controller) {
                (void)entity;
                controller.focused = event.focused;

                // Reset firstMouse when regaining focus to avoid camera jump
                if (event.focused) {
                    controller.firstMouse = true;
                }
            }
        );
    }

    void onMouseMovement(const MouseMovementEvent& event) {
        // Only process mouse movement when cursor is in window
        if (event.state == MouseCursorState::OUT_OF_WINDOW) return;

        registry.view<CameraComponent, CameraControllerComponent>().each(
            [&](Entity entity, CameraComponent& camera, CameraControllerComponent& controller) {
                (void)entity;

                if (!controller.focused) return;
                if (!controller.mouseControlEnabled) return;
                if (!camera.active) return;

                // Skip first mouse movement to avoid large jumps
                if (controller.firstMouse) {
                    controller.firstMouse = false;
                    return;
                }

                processMouse(camera, static_cast<float>(event.deltaX), static_cast<float>(event.deltaY));
            }
        );
    }

    void processMovement(TransformComponent& transform, CameraComponent& camera,
                         CameraMovementDirection direction, float deltaTime) {
        float velocity = camera.moveSpeed * deltaTime;

        switch (camera.mode) {
            case CameraMode::FPS:
                {
                    glm::vec3 front = camera.getFront();
                    glm::vec3 right = camera.getRight();

                    switch (direction) {
                        case CameraMovementDirection::FORWARD:
                            transform.position += front * velocity;
                            break;
                        case CameraMovementDirection::BACKWARD:
                            transform.position -= front * velocity;
                            break;
                        case CameraMovementDirection::LEFT:
                            transform.position -= right * velocity;
                            break;
                        case CameraMovementDirection::RIGHT:
                            transform.position += right * velocity;
                            break;
                        case CameraMovementDirection::UP:
                            transform.position.y += velocity;
                            break;
                        case CameraMovementDirection::DOWN:
                            transform.position.y -= velocity;
                            break;
                    }
                    transform.dirty = true;
                    camera.dirty = true;
                }
                break;

            case CameraMode::Orbit:
                // In orbit mode, movement adjusts orbit parameters
                switch (direction) {
                    case CameraMovementDirection::FORWARD:
                        camera.adjustOrbitDistance(velocity * 2.0f);
                        break;
                    case CameraMovementDirection::BACKWARD:
                        camera.adjustOrbitDistance(-velocity * 2.0f);
                        break;
                    case CameraMovementDirection::UP:
                        camera.orbitTarget.y += velocity;
                        camera.dirty = true;
                        break;
                    case CameraMovementDirection::DOWN:
                        camera.orbitTarget.y -= velocity;
                        camera.dirty = true;
                        break;
                    default:
                        break;
                }
                break;

            case CameraMode::Fixed:
                // Fixed cameras don't respond to movement input
                break;
        }
    }

    void processMouse(CameraComponent& camera, float xOffset, float yOffset) {
        switch (camera.mode) {
            case CameraMode::FPS:
            case CameraMode::Orbit:
                // yOffset already inverted in TestApplication callback
                camera.applyMouseDelta(xOffset, yOffset);
                break;

            case CameraMode::Fixed:
                // Fixed cameras don't respond to mouse input
                break;
        }
    }
};

} // namespace ECS

#endif //GAMEENGINE_ECS_CAMERA_INPUT_SYSTEM_H
