//
// Created by eceva on 1/14/2025.
//
#include "Entity.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

void Entity::render(glm::mat4 view, glm::mat4 projection) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    this->shader->use();
    this->shader->setMat4("view", view);
    this->shader->setMat4("projection", projection);
    this->shader->setVec3("shapeColor", glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 model = glm::mat4(1.0f);
    this->shader->setMat4("model", model);
}

void Entity::update(float deltaTime) {

}

