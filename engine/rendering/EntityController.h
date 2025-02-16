//
// Created by eceva on 1/14/2025.
//

#ifndef GAMEENGINE_ENTITYCONTROLLER_H
#define GAMEENGINE_ENTITYCONTROLLER_H

#include <vector>
#include "Entity.h"
#include "Renderer.h"
#include "memory"
#include "../entitities/Shape.h"

class EntityController : Renderer {
public:
    std::vector<Entity*> entities;

    int update(float deltaTime) override;
    void render(glm::mat4 view, glm::mat4 projection) override;
    void addEntity(Entity* entity);
    void debugRender(glm::mat4 view, glm::mat4 projection);
};

#endif //GAMEENGINE_ENTITYCONTROLLER_H
