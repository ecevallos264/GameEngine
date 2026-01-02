//
// Created by eceva on 1/14/2025.
//
#include <memory>
#include "EntityController.h"
#include "../physics/RigidBody.h"
#include "../entitities/Point.h"
#include "../camera/CameraHandler.h"

int EntityController::update(float deltaTime) {
//    for(Entity* entity: this->entities) {
//        entity->update(deltaTime);
//    }
    return 0;
}

void EntityController::render(glm::mat4 view, glm::mat4 projection) {
    float aspect = static_cast<float>(Settings::WINDOW_WIDTH) / Settings::WINDOW_HEIGHT;
    float fovY = 45.0f; // Example FOV
    float zNear = 1.0f;
//    float zFar = Settings::MAX_RENDER_DISTANCE;
    Frustum frustum = Frustum::createFrustumFromCamera(*CameraHandler::getInstance().getCamera(), aspect, fovY, zNear, /* zFar */ 9999);

//    frustum.drawFrusum(view, projection);
    debugRender(frustum, view, projection);
}

void EntityController::addEntity(Entity* entity) {
    this->entities.push_back(entity);
    buildBVH();
}

void EntityController::debugRender(Frustum frustum, glm::mat4 view, glm::mat4 projection) {
    if (bvhRoot) {
        bvhRoot->render(frustum, view, projection, 0);
    }
}

void EntityController::buildBVH() {
    if (bvhRoot) delete bvhRoot;  // Cleanup previous BVH if exists
    bvhRoot = new BVH::Node();    // Create a new root node

    for (Entity* entity : entities) {
        RigidBody* shape = dynamic_cast<RigidBody*>(entity);
        if (shape) {
            auto* volume = new BVH::BoundingVolume(shape);
            bvhRoot->insert(volume);
        }
    }
}

