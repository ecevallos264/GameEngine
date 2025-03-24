//
// Created by eceva on 1/14/2025.
//
#include <memory>
#include "EntityController.h"
#include "../physics/RigidBody.h"
#include "../entitities/Point.h"

int EntityController::update(float deltaTime) {
//    for(Entity* entity: this->entities) {
//        entity->update(deltaTime);
//    }
    return 0;
}

void EntityController::render(glm::mat4 view, glm::mat4 projection) {
//    for(Entity* entity: this->entities) {
//        entity->render(view, projection);
//    }
    debugRender(view, projection);
}

void EntityController::addEntity(Entity* entity) {
    this->entities.push_back(entity);
    buildBVH();
}

void EntityController::debugRender(glm::mat4 view, glm::mat4 projection) {
    // Render BVH tree for debugging
    if (bvhRoot) {
        bvhRoot->render(view, projection, 0);
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

