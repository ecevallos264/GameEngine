//
// Created by eceva on 1/15/2025.
//

#ifndef GAMEENGINE_CUBE_H
#define GAMEENGINE_CUBE_H

#include <glfw/glfw3.h>
#include "Rectangle.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

class Cube : public Rectangle {
private:
    std::function<void(double)> onUpdate;

public:
    Cube(glm::vec3 pos, Shader* shader, glm::vec3 color) : Rectangle(shader, pos, glm::vec3(1.0f), color) {}

    std::type_index getType() const {
        return typeid(Cube);
    }
};

#endif //GAMEENGINE_CUBE_H
