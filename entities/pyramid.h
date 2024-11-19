#ifndef SOFTWAREENGINEERINGPROJECT_PYRAMID_H
#define SOFTWAREENGINEERINGPROJECT_PYRAMID_H

#include "../shape.h"
#include <vector>
#include <glm/glm.hpp>

class Pyramid : public Shape {

public:
    Pyramid(glm::vec3 pos) {
        position = pos;
        vertices = {
                -0.5f, -0.5f, -0.5f,    1.0f, 0.0f, 0.0f,    1.0f,
                0.5f, -0.5f, -0.5f,     0.0f, 1.0f, 0.0f,    1.0f,
                0.5f, -0.5f,  0.5f,     0.0f, 0.0f, 1.0f,    1.0f,
                -0.5f, -0.5f,  0.5f,    1.0f, 1.0f, 0.0f,    1.0f,
                0.0f,  0.5f,  0.0f,     1.0f, 0.0f, 1.0f,    1.0f,
        };

        indices = {
                0, 1, 2,
                2, 3, 0,

                0, 1, 4,
                1, 2, 4,
                2, 3, 4,
                3, 0, 4
        };
    }
};

#endif //SOFTWAREENGINEERINGPROJECT_PYRAMID_H
