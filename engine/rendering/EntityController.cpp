//
// Created by eceva on 1/14/2025.
//
#include <memory>
#include "EntityController.h"
#include "../physics/RigidBody.h"
#include "../entitities/Point.h"

int EntityController::update(float deltaTime) {
    for(Entity* entity: this->entities) {
        entity->update(deltaTime);
    }
    return 0;
}

void EntityController::render(glm::mat4 view, glm::mat4 projection) {
    for(Entity* entity: this->entities) {
        entity->render(view, projection);
    }
}

void EntityController::addEntity(Entity* entity) {
    this->entities.push_back(entity);
}

void EntityController::debugRender(glm::mat4 view, glm::mat4 projection) {
    for(Entity* entity: this->entities) {
        RigidBody* shape = static_cast<RigidBody*>(entity);
        for (const auto& vertex : shape->getVertices()) {
            Point point(vertex.position, glm::vec3(1.0f, 0.0f, 0.0f), shape->shader); // Red point for debugging
            point.render(view, projection);
        }
    }

}

