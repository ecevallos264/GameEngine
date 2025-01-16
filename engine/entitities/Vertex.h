//
// Created by eceva on 1/15/2025.
//

#ifndef GAMEENGINE_VERTEX_H
#define GAMEENGINE_VERTEX_H

#include "glm/vec3.hpp"

struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
    float alpha;

public:
    Vertex(const glm::vec3& pos, const glm::vec3& col, float a = 1.0f)
            : position(pos), color(col), alpha(a) {}

};

#endif //GAMEENGINE_VERTEX_H
