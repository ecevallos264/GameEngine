#ifndef SOFTWAREENGINEERINGPROJECT_HEX_PRISM_H
#define SOFTWAREENGINEERINGPROJECT_HEX_PRISM_H

#include "../shape.h"

class HexagonalPrism : public Shape {
public:
    HexagonalPrism(glm::vec3 pos) {
        position = pos;
        vertices = {
            0.0f, -0.5f,  0.0f,         1.0f, 0.0f, 0.0f,    1.0f,
            0.5f, -0.5f,  0.0f,         0.0f, 1.0f, 0.0f,    1.0f,
            0.25f, -0.5f,  0.433f,      0.0f, 0.0f, 1.0f,    1.0f,
            -0.25f, -0.5f,  0.433f,     1.0f, 1.0f, 0.0f,    1.0f,
            -0.5f, -0.5f,  0.0f,        1.0f, 0.0f, 1.0f,    1.0f,
            -0.25f, -0.5f, -0.433f,     0.0f, 1.0f, 1.0f,    1.0f,
            0.25f, -0.5f, -0.433f,      0.5f, 0.5f, 0.5f,    1.0f,

            0.0f,  0.5f,  0.0f,         1.0f, 0.5f, 0.0f,    1.0f,
            0.5f,  0.5f,  0.0f,         0.0f, 1.0f, 0.5f,    1.0f,
            0.25f,  0.5f,  0.433f,      0.5f, 0.0f, 1.0f,    1.0f,
            -0.25f,  0.5f,  0.433f,     1.0f, 0.5f, 0.5f,    1.0f,
            -0.5f,  0.5f,  0.0f,        0.5f, 1.0f, 0.5f,    1.0f,
            -0.25f,  0.5f, -0.433f,     0.5f, 0.5f, 1.0f,    1.0f,
            0.25f,  0.5f, -0.433f,      1.0f, 1.0f, 1.0f,    1.0f,
        };

        indices = {
            0, 1, 2,
            0, 2, 3,
            0, 3, 4,
            0, 4, 5,
            0, 5, 6,
            0, 6, 1,

            7, 8, 9,
            7, 9, 10,
            7, 10, 11,
            7, 11, 12,
            7, 12, 13,
            7, 13, 8,

            1, 8, 9,
            1, 9, 2,
            2, 9, 10,
            2, 10, 3,
            3, 10, 11,
            3, 11, 4,
            4, 11, 12,
            4, 12, 5,
            5, 12, 13,
            5, 13, 6,
            6, 13, 8,
            6, 8, 1
        };
    }

    HexagonalPrism(glm::vec3 pos, glm::vec3 color) {
        position = pos;
        vertices = {
                0.0f, -0.5f,  0.0f,         color.x, color.y, color.z,    1.0f,
                0.5f, -0.5f,  0.0f,         color.x, color.y, color.z,    1.0f,
                0.25f, -0.5f,  0.433f,      color.x, color.y, color.z,    1.0f,
                -0.25f, -0.5f,  0.433f,     color.x, color.y, color.z,    1.0f,
                -0.5f, -0.5f,  0.0f,        color.x, color.y, color.z,    1.0f,
                -0.25f, -0.5f, -0.433f,     color.x, color.y, color.z,    1.0f,
                0.25f, -0.5f, -0.433f,      color.x, color.y, color.z,    1.0f,

                0.0f,  0.5f,  0.0f,         color.x, color.y, color.z,    1.0f,
                0.5f,  0.5f,  0.0f,         color.x, color.y, color.z,    1.0f,
                0.25f,  0.5f,  0.433f,      color.x, color.y, color.z,    1.0f,
                -0.25f,  0.5f,  0.433f,     color.x, color.y, color.z,    1.0f,
                -0.5f,  0.5f,  0.0f,        color.x, color.y, color.z,    1.0f,
                -0.25f,  0.5f, -0.433f,     color.x, color.y, color.z,    1.0f,
                0.25f,  0.5f, -0.433f,      color.x, color.y, color.z,    1.0f,
        };

        indices = {
                0, 1, 2,
                0, 2, 3,
                0, 3, 4,
                0, 4, 5,
                0, 5, 6,
                0, 6, 1,

                7, 8, 9,
                7, 9, 10,
                7, 10, 11,
                7, 11, 12,
                7, 12, 13,
                7, 13, 8,

                1, 8, 9,
                1, 9, 2,
                2, 9, 10,
                2, 10, 3,
                3, 10, 11,
                3, 11, 4,
                4, 11, 12,
                4, 12, 5,
                5, 12, 13,
                5, 13, 6,
                6, 13, 8,
                6, 8, 1
        };
    }

    void draw() override {
        glBindVertexArray(this->getVAO());
        glDrawElements(GL_TRIANGLES, this->getIndices().size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
};

#endif //SOFTWAREENGINEERINGPROJECT_HEX_PRISM_H