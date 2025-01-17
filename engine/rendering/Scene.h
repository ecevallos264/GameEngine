//
// Created by eceva on 1/14/2025.
//

#ifndef GAMEENGINE_SCENE_H
#define GAMEENGINE_SCENE_H

#include "Renderer.h"
#include "EntityController.h"

class Scene : Renderer {
protected:
    EntityController* entityController = new EntityController();
public:
    void update(float deltaTime) override;
    void render(glm::mat4 view, glm::mat4 projection) override;
    void addEntity(Entity* entity);
    EntityController* getEntityController() {
        return this->entityController;
    }
};

#endif //GAMEENGINE_SCENE_H
