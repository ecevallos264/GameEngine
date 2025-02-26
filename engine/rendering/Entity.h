
//
// Created by eceva on 1/14/2025.
//

#ifndef GAMEENGINE_ENTITY_H
#define GAMEENGINE_ENTITY_H

#include <atomic>
#include <typeindex>
#include "Renderer.h"
#include "../core/shaders/shader-compiler.h"
#include "../core/state/FlagState.h"

class Entity : public Renderer {
public:
    FlagState* flags = new FlagState();
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
    int update(float deltaTime) override;


    static std::type_index getType() {
        return typeid(Entity);
    };

    bool isFixed() const {
        return fixed;
    }


};

#endif //GAMEENGINE_ENTITY_H
