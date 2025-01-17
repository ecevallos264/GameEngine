//
// Created by eceva on 1/14/2025.
//
#include <memory>
#include "EntityController.h"
#include "../entitities/shape.h"

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

