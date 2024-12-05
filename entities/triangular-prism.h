#ifndef SOFTWAREENGINEERINGPROJECT_TRIANGULAR_PRISM_H
#define SOFTWAREENGINEERINGPROJECT_TRIANGULAR_PRISM_H

#include "../shape.h"
#include <vector>
#include <glm/glm.hpp>

class TriangularPrism : public Shape {

public:
    explicit TriangularPrism(glm::vec3 pos) {
        position = pos;
        vertices = {
            -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,    1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,    1.0f,
             0.0f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,    1.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,    1.0f,
             0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f,    1.0f,
             0.0f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f,    1.0f,
        };

        indices = {
            0, 1, 2,

            3, 4, 5,

            0, 1, 4,  4, 3, 0,
            1, 2, 5,  5, 4, 1,
            2, 0, 3,  3, 5, 2
        };
    }

    explicit TriangularPrism(glm::vec3 pos, glm::vec3 color) {
        position = pos;
        vertices = {
                -0.5f, -0.5f,  0.5f,    color.x, color.y, color.z,    1.0f,
                0.5f, -0.5f,  0.5f,     color.x, color.y, color.z,    1.0f,
                0.0f, -0.5f, -0.5f,     color.x, color.y, color.z,    1.0f,

                -0.5f,  0.5f,  0.5f,    color.x, color.y, color.z,    1.0f,
                0.5f,  0.5f,  0.5f,     color.x, color.y, color.z,    1.0f,
                0.0f,  0.5f, -0.5f,     color.x, color.y, color.z,    1.0f,
        };

        indices = {
                0, 1, 2,

                3, 4, 5,

                0, 1, 4,  4, 3, 0,
                1, 2, 5,  5, 4, 1,
                2, 0, 3,  3, 5, 2
        };
    }

    void draw() override {
        glBindVertexArray(this->getVAO());
        glDrawElements(GL_TRIANGLES, this->getIndices().size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
};

#endif //SOFTWAREENGINEERINGPROJECT_TRIANGULAR_PRISM_H