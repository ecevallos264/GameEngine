#ifndef GAMEENGINE_ECS_CAMERA_SYSTEM_H
#define GAMEENGINE_ECS_CAMERA_SYSTEM_H

#include "../../runtime/ISystem.h"
#include "../Registry.h"
#include "../components/CameraComponent.h"
#include "../components/TransformComponent.h"
#include "../../core/settings/settings.h"
#include "../../core/math/Frustrum.h"

namespace ECS {

class CameraSystem : public ISystem {
public:
    explicit CameraSystem(Registry& registry) : registry(registry) {}

    void initialize(SystemContext& context) override {
        (void)context;
        // Set initial aspect ratio from settings
        aspectRatio = static_cast<float>(Settings::WINDOW_WIDTH) /
                      static_cast<float>(Settings::WINDOW_HEIGHT);

        // Update all cameras with initial aspect ratio
        registry.view<CameraComponent>().each(
            [this](Entity entity, CameraComponent& camera) {
                (void)entity;
                camera.aspectRatio = aspectRatio;
            }
        );
    }

    void update(SystemContext& context) override {
        (void)context;

        // Update all camera matrices
        registry.view<TransformComponent, CameraComponent>().each(
            [this](Entity entity, TransformComponent& transform, CameraComponent& camera) {
                (void)entity;
                updateCameraMatrices(transform, camera);
            }
        );

        // Find and cache the active main camera
        findActiveCamera();
    }

    // Get the currently active camera entity
    Entity getActiveCamera() const {
        return activeCameraEntity;
    }

    // Get camera by tag
    Entity getCameraByTag(CameraTag tag) {
        Entity found;
        registry.view<CameraComponent>().each(
            [&](Entity entity, CameraComponent& camera) {
                if (camera.tag == tag && camera.active) {
                    found = entity;
                }
            }
        );
        return found;
    }

    // Get view matrix of active camera
    glm::mat4 getViewMatrix() const {
        if (!activeCameraEntity.isValid()) return glm::mat4(1.0f);
        auto* cam = registry.get<CameraComponent>(activeCameraEntity);
        return cam ? cam->viewMatrix : glm::mat4(1.0f);
    }

    // Get projection matrix of active camera
    glm::mat4 getProjectionMatrix() const {
        if (!activeCameraEntity.isValid()) return glm::mat4(1.0f);
        auto* cam = registry.get<CameraComponent>(activeCameraEntity);
        return cam ? cam->projectionMatrix : glm::mat4(1.0f);
    }

    // Get view-projection matrix of active camera
    glm::mat4 getViewProjectionMatrix() const {
        if (!activeCameraEntity.isValid()) return glm::mat4(1.0f);
        auto* cam = registry.get<CameraComponent>(activeCameraEntity);
        return cam ? cam->viewProjectionMatrix : glm::mat4(1.0f);
    }

    // Get position of active camera
    glm::vec3 getCameraPosition() const {
        if (!activeCameraEntity.isValid()) return glm::vec3(0.0f);
        auto* transform = registry.get<TransformComponent>(activeCameraEntity);
        return transform ? transform->position : glm::vec3(0.0f);
    }

    // Get front vector of active camera
    glm::vec3 getCameraFront() const {
        if (!activeCameraEntity.isValid()) return glm::vec3(0.0f, 0.0f, -1.0f);
        auto* cam = registry.get<CameraComponent>(activeCameraEntity);
        return cam ? cam->getFront() : glm::vec3(0.0f, 0.0f, -1.0f);
    }

    // Get frustum data for active camera (for culling)
    bool getActiveFrustum(Frustum& outFrustum) const {
        if (!activeCameraEntity.isValid()) return false;
        auto* cam = registry.get<CameraComponent>(activeCameraEntity);
        auto* transform = registry.get<TransformComponent>(activeCameraEntity);
        if (!cam || !transform) return false;

        outFrustum = Frustum::createFrustumFromParameters(
            transform->position,
            cam->getFront(),
            cam->getUp(),
            cam->aspectRatio,
            cam->fov,
            cam->nearPlane,
            cam->farPlane
        );
        return true;
    }

    // Update aspect ratio (call on window resize)
    void updateAspectRatio(float width, float height) {
        aspectRatio = width / height;
        registry.view<CameraComponent>().each(
            [this](Entity entity, CameraComponent& camera) {
                (void)entity;
                camera.aspectRatio = aspectRatio;
                camera.dirty = true;
            }
        );
    }

    // Set active camera by entity
    void setActiveCamera(Entity entity) {
        // Deactivate all cameras with Main tag
        registry.view<CameraComponent>().each(
            [](Entity e, CameraComponent& cam) {
                (void)e;
                if (cam.tag == CameraTag::Main) {
                    cam.active = false;
                }
            }
        );

        // Activate the specified camera
        auto* cam = registry.get<CameraComponent>(entity);
        if (cam) {
            cam->active = true;
            cam->tag = CameraTag::Main;
            activeCameraEntity = entity;
        }
    }

private:
    Registry& registry;
    Entity activeCameraEntity;
    float aspectRatio = 16.0f / 9.0f;

    void updateCameraMatrices(TransformComponent& transform, CameraComponent& camera) {
        glm::vec3 position = transform.position;
        glm::vec3 front, up;

        switch (camera.mode) {
            case CameraMode::FPS:
                // FPS mode: use yaw/pitch for orientation
                front = camera.getFront();
                up = camera.getUp();
                camera.viewMatrix = glm::lookAt(position, position + front, up);
                break;

            case CameraMode::Orbit:
                // Orbit mode: position camera around target
                {
                    float camX = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch)) * camera.orbitDistance;
                    float camY = sin(glm::radians(camera.pitch)) * camera.orbitDistance;
                    float camZ = cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch)) * camera.orbitDistance;

                    glm::vec3 orbitPos = camera.orbitTarget + glm::vec3(camX, camY, camZ);
                    transform.position = orbitPos;  // Update transform to match orbit position
                    camera.viewMatrix = glm::lookAt(orbitPos, camera.orbitTarget, glm::vec3(0.0f, 1.0f, 0.0f));
                }
                break;

            case CameraMode::Fixed:
                // Fixed mode: use TransformComponent rotation directly
                {
                    glm::mat4 rotMat = glm::mat4(1.0f);
                    rotMat = glm::rotate(rotMat, glm::radians(transform.rotation.z), glm::vec3(0, 0, 1));
                    rotMat = glm::rotate(rotMat, glm::radians(transform.rotation.y), glm::vec3(0, 1, 0));
                    rotMat = glm::rotate(rotMat, glm::radians(transform.rotation.x), glm::vec3(1, 0, 0));

                    front = glm::vec3(rotMat * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
                    up = glm::vec3(rotMat * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
                    camera.viewMatrix = glm::lookAt(position, position + front, up);
                }
                break;
        }

        // Compute projection matrix
        if (camera.projection == CameraProjection::Perspective) {
            camera.projectionMatrix = glm::perspective(
                glm::radians(camera.fov),
                camera.aspectRatio,
                camera.nearPlane,
                camera.farPlane
            );
        } else {
            float halfWidth = camera.orthoSize * camera.aspectRatio;
            float halfHeight = camera.orthoSize;
            camera.projectionMatrix = glm::ortho(
                -halfWidth, halfWidth,
                -halfHeight, halfHeight,
                camera.nearPlane, camera.farPlane
            );
        }

        // Compute combined matrix
        camera.viewProjectionMatrix = camera.projectionMatrix * camera.viewMatrix;

        camera.dirty = false;
    }

    void findActiveCamera() {
        Entity bestCamera;
        int highestPriority = -1;

        registry.view<CameraComponent>().each(
            [&](Entity entity, CameraComponent& camera) {
                if (camera.active && camera.tag == CameraTag::Main) {
                    if (camera.priority > highestPriority) {
                        highestPriority = camera.priority;
                        bestCamera = entity;
                    }
                }
            }
        );

        // If no Main camera found, try to find any active camera
        if (!bestCamera.isValid()) {
            registry.view<CameraComponent>().each(
                [&](Entity entity, CameraComponent& camera) {
                    if (camera.active && !bestCamera.isValid()) {
                        bestCamera = entity;
                    }
                }
            );
        }

        activeCameraEntity = bestCamera;
    }
};

} // namespace ECS

#endif //GAMEENGINE_ECS_CAMERA_SYSTEM_H
