
//
// Created by eceva on 1/14/2025.
//

#ifndef GAMEENGINE_ENTITY_H
#define GAMEENGINE_ENTITY_H

#include <atomic>
#include <typeindex>
#include "Renderer.h"
#include "../core/shaders/shader-compiler.h"

class Entity : public Renderer {
public:
    std::string id;
    bool fixed = true;

    glm::vec3 color;
    Shader* shader;
    static std::atomic<size_t> nextId;
    size_t instanceId;


public:
    size_t getInstanceId() const {
        return instanceId;
    }

    Entity(Shader* shader) : shader(shader) {
    }
    void render(glm::mat4 view, glm::mat4 projection) override;
    void update(float deltaTime) override;


    static std::type_index getType() {
        return typeid(Entity);
    };


};

#endif //GAMEENGINE_ENTITY_H
