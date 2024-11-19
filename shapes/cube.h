//
// Created by eceva on 11/19/2024.
//

#ifndef SOFTWAREENGINEERINGPROJECT_CUBE_H
#define SOFTWAREENGINEERINGPROJECT_CUBE_H

#include "../shape.h"

class Cube : public Shape {

private:
    std::vector<float> vertices = {
            // Front face
            -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f
    };

    std::vector<unsigned int> indices = {
            0, 1, 2,
            2, 3, 0,

            4, 5, 6,
            6, 7, 4,

            0, 3, 7,
            7, 4, 0,

            1, 5, 6,
            6, 2, 1,

            3, 2, 6,
            6, 7, 3,

            0, 1, 5,
            5, 4, 0
    };


public:
    Cube(glm::vec3 pos) {
        position = pos;
    };
    std::vector<float> getVertices() override {
        return  vertices;
    }

    std::vector<unsigned int> getIndices() override {
        return  indices;
    }
};

#endif //SOFTWAREENGINEERINGPROJECT_CUBE_H
