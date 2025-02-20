//
// Created by eceva on 1/14/2025.
//
#include "Scene.h"

int Scene::update(float deltaTime) {
//    OctTree::getInstance().getRoot()->update(deltaTime);
    this->root->processPending();
    this->root->update(deltaTime);
    return 0;
}

void Scene::render(glm::mat4 view, glm::mat4 projection) {
    if(this->root != nullptr) {
        this->root->render(view, projection);
    }
}

void Scene::addEntity(Shape* entity, std::string name) {
    if(this->root != nullptr) {
        if(!this->root->region->contains(entity->boundingBox)) {
            this->root = new Octree(BoundingBox::combine(this->root->region, entity->boundingBox));
        }
    } else {
        this->root = new Octree(entity->boundingBox);
    }
    this->root->addToPending(entity);
}