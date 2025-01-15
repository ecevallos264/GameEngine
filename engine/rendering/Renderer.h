//
// Created by eceva on 1/14/2025.
//

#ifndef GAMEENGINE_RENDERER_H
#define GAMEENGINE_RENDERER_H

#include "glm/fwd.hpp"

class Renderer {
    virtual void update(float deltaTime) = 0;
    virtual void render(glm::mat4 view, glm::mat4 projection) = 0;
};

#endif //GAMEENGINE_RENDERER_H
