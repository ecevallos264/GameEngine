//
// Created by eceva on 1/14/2025.
//

#ifndef GAMEENGINE_ENTITY_H
#define GAMEENGINE_ENTITY_H

#include "Renderer.h"
#include "../core/shaders/shader-compiler.h"

class Entity : Renderer {
protected:
    Shader* shader;
public:
    Entity(Shader* shader) : shader(shader) {}
    void render(glm::mat4 view, glm::mat4 projection) override;
    void update(float deltaTime) override;
};

#endif //GAMEENGINE_ENTITY_H
