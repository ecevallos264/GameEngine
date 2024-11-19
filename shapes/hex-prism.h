//
// Created by eceva on 11/19/2024.
//

#ifndef SOFTWAREENGINEERINGPROJECT_HEX_PRISM_H
#define SOFTWAREENGINEERINGPROJECT_HEX_PRISM_H

#include "../shape.h"

class HexagonalPrism : public Shape {

private:
    std::vector<float> vertices = {
            0.0f, -0.5f,  0.0f,  1.0f, 0.0f, 0.0f,
            0.5f, -0.5f,  0.0f,  0.0f, 1.0f, 0.0f,
            0.25f, -0.5f,  0.433f,  0.0f, 0.0f, 1.0f,
            -0.25f, -0.5f,  0.433f,  1.0f, 1.0f, 0.0f,
            -0.5f, -0.5f,  0.0f,  1.0f, 0.0f, 1.0f,
            -0.25f, -0.5f, -0.433f,  0.0f, 1.0f, 1.0f,
            0.25f, -0.5f, -0.433f,  0.5f, 0.5f, 0.5f,

            0.0f,  0.5f,  0.0f,  1.0f, 0.5f, 0.0f,
            0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 0.5f,
            0.25f,  0.5f,  0.433f,  0.5f, 0.0f, 1.0f,
            -0.25f,  0.5f,  0.433f,  1.0f, 0.5f, 0.5f,
            -0.5f,  0.5f,  0.0f,  0.5f, 1.0f, 0.5f,
            -0.25f,  0.5f, -0.433f,  0.5f, 0.5f, 1.0f,
            0.25f,  0.5f, -0.433f,  1.0f, 1.0f, 1.0f
    };

    std::vector<unsigned int> indices = {
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


public:
    HexagonalPrism(glm::vec3 pos) {
        position = pos;
    };
    std::vector<float> getVertices() override {
        return  vertices;
    }

    std::vector<unsigned int> getIndices() override {
        return  indices;
    }
};

#endif //SOFTWAREENGINEERINGPROJECT_HEX_PRISM_H
