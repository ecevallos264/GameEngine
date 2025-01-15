//
// Created by eceva on 1/14/2025.
//
#include "Entity.h"

void Entity::render(glm::mat4 view, glm::mat4 projection) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    this->shader->use();
    this->shader->setMat4("view", view);
    this->shader->setMat4("projection", projection);
    this->shader->setVec3("shapeColor", glm::vec3(1.0f, 0.0f, 0.0f));
}

void Entity::update(float deltaTime) {

}

