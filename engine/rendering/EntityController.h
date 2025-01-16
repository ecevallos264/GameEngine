//
// Created by eceva on 1/14/2025.
//

#ifndef GAMEENGINE_ENTITYCONTROLLER_H
#define GAMEENGINE_ENTITYCONTROLLER_H

#include <vector>
#include "Entity.h"
#include "Renderer.h"
#include "memory"
#include "../entitities/shape.h"

class EntityController : Renderer {
public:
    std::vector<std::shared_ptr<Entity>> entities;

    void update(float deltaTime) override;
    void render(glm::mat4 view, glm::mat4 projection) override;
    void addEntity(std::shared_ptr<Entity> entity);
};

#endif //GAMEENGINE_ENTITYCONTROLLER_H
