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
private:
    std::function<void(double)> onUpdate;

public:
    Cube(glm::vec3 pos, Shader* shader, glm::vec3 color) : Shape(shader) {
        position = pos;
        this->vertices.push_back({glm::vec3(-0.5f, -0.5f,  0.5f), color, 1.0f});
        this->vertices.push_back({glm::vec3(0.5f, -0.5f,  0.5f), color, 1.0f});
        this->vertices.push_back({glm::vec3(0.5f, 0.5f,  0.5f), color, 1.0f});
        this->vertices.push_back({glm::vec3(-0.5f, 0.5f,  0.5f), color, 1.0f});

        this->vertices.push_back({glm::vec3(-0.5f, -0.5f,  -0.5f), color, 1.0f});
        this->vertices.push_back({glm::vec3(0.5f, -0.5f,  -0.5f), color, 1.0f});
        this->vertices.push_back({glm::vec3(0.5f, 0.5f,  -0.5f), color, 1.0f});
        this->vertices.push_back({glm::vec3(-0.5f, 0.5f,  -0.5f), color, 1.0f});
        updateVertexBuffer();

        indices = {
                0, 1, 2, 2, 3, 0,
                4, 5, 6, 6, 7, 4,
                0, 3, 7, 7, 4, 0,
                1, 5, 6, 6, 2, 1,
                3, 2, 6, 6, 7, 3,
                0, 1, 5, 5, 4, 0
        };
        initializeBuffers();
    }

    void render(glm::mat4 view, glm::mat4 projection) override {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, this->getPosition());

        this->shader->use();
        this->shader->setMat4("view", view);
        this->shader->setMat4("projection", projection);
        this->shader->setMat4("model", model);
        this->shader->setVec3("shapeColor", glm::vec3(1.0f, 0.0f, 0.0f));

        glBindVertexArray(this->getVAO());
        glDrawElements(GL_TRIANGLES, this->getIndices().size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void setOnUpdateCallback(std::function<void(double)> callback) {
        this->onUpdate = callback;
    }

    void update(float deltaTime) override {
        Shape::update(deltaTime);
        if(this->onUpdate) {
            this->onUpdate(deltaTime);
        }
    }

    std::type_index getType() const {
        return typeid(Cube);
    }
};

#endif //GAMEENGINE_CUBE_H
