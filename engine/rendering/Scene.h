//
// Created by eceva on 1/14/2025.
//

#ifndef GAMEENGINE_SCENE_H
#define GAMEENGINE_SCENE_H

#include "Renderer.h"
#include "EntityController.h"
#include "../core/structures/Octree.h"

class Scene : Renderer {
protected:
    Octree* root;
    EntityController* entityController = new EntityController();
public:
    virtual void setup() = 0;
    int update(float deltaTime) override;
    void render(glm::mat4 view, glm::mat4 projection) override;
    void addEntity(Shape* entity, std::string name);
    EntityController* getEntityController() {
        return this->entityController;
    }

    EntityController* getController() {
        return this->entityController;
    }
};

#endif //GAMEENGINE_SCENE_H
