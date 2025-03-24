//
// Created by eceva on 1/14/2025.
//
#include "Scene.h"

int Scene::update(float deltaTime) {
//    OctTree::getInstance().getRoot()->update(deltaTime);
    this->entityController->update(deltaTime);
    return 0;
}

void Scene::render(glm::mat4 view, glm::mat4 projection) {
    this->entityController->render(view, projection);
}

void Scene::addEntity(RigidBody* entity, std::string name) {
    std::cout << "Added " << name << std::endl;
    this->entityController->addEntity(entity);
}