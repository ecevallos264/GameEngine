//
// Created by eceva on 1/14/2025.
//
#include <memory>
#include "EntityController.h"
#include "../entitities/shape.h"
#include "../entitities/Point.h"

void EntityController::update(float deltaTime) {
    for(Entity* entity: this->entities) {
        entity->update(deltaTime);
    }
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
        Shape* shape = static_cast<Shape*>(entity);
        for (const auto& vertex : shape->vertices) {
            Point point(vertex.position, glm::vec3(1.0f, 0.0f, 0.0f), shape->shader); // Red point for debugging
            point.render(view, projection);
        }
    }

}

