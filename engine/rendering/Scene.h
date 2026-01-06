//
// Created by eceva on 1/14/2025.
//

#ifndef GAMEENGINE_SCENE_H
#define GAMEENGINE_SCENE_H

#include "Renderer.h"
#include "../ecs/Registry.h"

class Window;  // Forward declaration

class Scene : Renderer {
protected:
    ECS::Registry registry;

public:
    virtual ~Scene() = default;

    virtual void setup() = 0;
    int update(float deltaTime) override;
    void render(glm::mat4 view, glm::mat4 projection) override;

    // Handle input using the abstracted Window class
    virtual int handleInput(Window* window) = 0;

    // ECS Registry access
    ECS::Registry& getRegistry() { return registry; }
    const ECS::Registry& getRegistry() const { return registry; }

    // Create an entity in this scene
    ECS::Entity createEntity() { return registry.create(); }

    // Destroy an entity
    void destroyEntity(ECS::Entity entity) { registry.destroy(entity); }
};

#endif //GAMEENGINE_SCENE_H
