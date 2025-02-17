//
// Created by eceva on 1/14/2025.
//
#include "Scene.h"
#include "../core/structures/Oct-Tree.h"

int Scene::update(float deltaTime) {
    OctTree::getInstance().getRoot()->update(deltaTime);
    return 0;
}

void Scene::render(glm::mat4 view, glm::mat4 projection) {
    OctTree::getInstance().getRoot()->render(view, projection);
}

void Scene::addEntity(Shape* entity, std::string name) {
//    this->entityController->addEntity(entity);
    OctTree::getInstance().insert(entity);
}