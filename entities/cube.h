#ifndef SOFTWAREENGINEERINGPROJECT_CUBE_H
#define SOFTWAREENGINEERINGPROJECT_CUBE_H

#include "../shape.h"

class Cube : public Shape {

public:
    Cube(glm::vec3 pos) {
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

    Cube(glm::vec3 pos, glm::vec3 color) {
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

    void draw() override {
        glBindVertexArray(this->getVAO());
        glDrawElements(GL_TRIANGLES, this->getIndices().size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void update(float deltaTime, Shader* shader) override {
        float time = glfwGetTime();
        Shape::update(deltaTime, shader);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, this->getPosition());
        model = glm::rotate(model, (float)time * glm::radians(50.0f), glm::vec3(0.0, 0.0, 1.0));
        shader->setMat4("model", model);
    }
};

#endif //SOFTWAREENGINEERINGPROJECT_CUBE_H