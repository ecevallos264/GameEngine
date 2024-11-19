#ifndef SOFTWAREENGINEERINGPROJECT_CUBE_H
#define SOFTWAREENGINEERINGPROJECT_CUBE_H

#include "../shape.h"

class Cube : public Shape {
public:
    Cube(glm::vec3 pos) {
        position = pos;
        vertices = {
            -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f
        };

        indices = {
            0, 1, 2, 2, 3, 0,
            4, 5, 6, 6, 7, 4,
            0, 3, 7, 7, 4, 0,
            1, 5, 6, 6, 2, 1,
            3, 2, 6, 6, 7, 3,
            0, 1, 5, 5, 4, 0
        };
    }
};

#endif //SOFTWAREENGINEERINGPROJECT_CUBE_H