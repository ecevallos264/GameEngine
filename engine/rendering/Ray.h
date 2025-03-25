//
// Created by eceva on 3/24/2025.
//

#ifndef GAMEENGINE_RAY_H
#define GAMEENGINE_RAY_H

#include "RenderableEntity.h"c
#include "../core/shaders/ShaderManager.h"

class Ray : public RenderableEntity {
private:
    glm::vec3 origin;
    glm::vec3 direction;
public:
    Ray(const glm::vec3& o, const glm::vec3& d) : RenderableEntity(ShaderManager::getInstance().getShader("shader1")), origin(o), direction(glm::normalize(d)) {}
};

#endif //GAMEENGINE_RAY_H
