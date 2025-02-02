//
// Created by eceva on 1/14/2025.
//
#include "Scene.h"

void Scene::update(float deltaTime) {
    this->entityController->update(deltaTime);
}

void Scene::render(glm::mat4 view, glm::mat4 projection) {
    this->entityController->render(view, projection);
}

void Scene::addEntity(Entity* entity, std::string name) {
    this->entityController->addEntity(entity);
}