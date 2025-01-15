//
// Created by eceva on 1/15/2025.
//

#ifndef GAMEENGINE_CUBE_H
#define GAMEENGINE_CUBE_H

#include <glfw/glfw3.h>
#include "shape.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

class Cube : public Shape {

public:
    Cube(glm::vec3 pos, Shader* shader) : Shape(shader) {
        position = pos;
        vertices = {
                -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f,
                0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f,
                0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 1.0f,
                -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 1.0f,

                -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f, 1.0f,
                0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 1.0f,
                0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f, 1.0f,
                -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 1.0f,
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

    Cube(glm::vec3 pos, glm::vec3 color, Shader* shader) : Shape(shader) {
        position = pos;
        vertices = {
                -0.5f, -0.5f,  0.5f,    color.x, color.y, color.z, 1.0f,
                0.5f, -0.5f,  0.5f,     color.x, color.y, color.z, 1.0f,
                0.5f,  0.5f,  0.5f,     color.x, color.y, color.z, 1.0f,
                -0.5f,  0.5f,  0.5f,    color.x, color.y, color.z, 1.0f,

                -0.5f, -0.5f, -0.5f,    color.x, color.y, color.z, 1.0f,
                0.5f, -0.5f, -0.5f,     color.x, color.y, color.z, 1.0f,
                0.5f,  0.5f, -0.5f,     color.x, color.y, color.z, 1.0f,
                -0.5f,  0.5f, -0.5f,    color.x, color.y, color.z, 1.0f,
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

    void render(glm::mat4 view, glm::mat4 projection) override {
        Shape::render(view, projection);
        glBindVertexArray(this->getVAO());
//
        this->shader->setVec3("shapeColor", glm::vec3(1.0f, 0.0f, 0.0f));

        glDrawElements(GL_TRIANGLES, this->getIndices().size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR) {
            std::cerr << "OpenGL error: " << err << std::endl;
        }
    }

    void update(float deltaTime) override {
        Shape::update(deltaTime);
        glm::mat4 model = glm::mat4(1.0f);
        this->shader->setMat4("model", model);
    }
};

#endif //GAMEENGINE_CUBE_H
