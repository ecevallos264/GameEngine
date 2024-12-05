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

    Pyramid(glm::vec3 pos, glm::vec3 color) {
        position = pos;
        vertices = {
                -0.5f, -0.5f, -0.5f,    color.x, color.y, color.z,    1.0f,
                0.5f, -0.5f, -0.5f,     color.x, color.y, color.z,    1.0f,
                0.5f, -0.5f,  0.5f,     color.x, color.y, color.z,    1.0f,
                -0.5f, -0.5f,  0.5f,    color.x, color.y, color.z,    1.0f,
                0.0f,  0.5f,  0.0f,     color.x, color.y, color.z,    1.0f,
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

    void draw() override {
        glBindVertexArray(this->getVAO());
        glDrawElements(GL_TRIANGLES, this->getIndices().size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
};

#endif //SOFTWAREENGINEERINGPROJECT_PYRAMID_H
