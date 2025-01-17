// Created by eceva on 1/15/2025.

#ifndef GAMEENGINE_POINT_H
#define GAMEENGINE_POINT_H

#include <glm/vec3.hpp>
#include <iostream>
#include "shape.h"

class Point : public Shape {
private:
    glm::vec3 color;

public:
    Point(glm::vec3 pos, glm::vec3 color, Shader* shader) : Shape(shader), color(color) {
        position = pos;
        this->vertices.push_back({pos, color, 1.0f});
        updateVertexBuffer();

        indices = { 0 }; // A point has a single index
        initializeBuffers();
    }

    void render(glm::mat4 view, glm::mat4 projection) override {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glPointSize(10.0f);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, this->getPosition());

        this->shader->use();
        this->shader->setMat4("view", view);
        this->shader->setMat4("projection", projection);
        this->shader->setMat4("model", model);
        this->shader->setVec3("shapeColor", color);

        glBindVertexArray(this->getVAO());
        glDrawArrays(GL_POINTS, 0, 1); // Render as a single point
        glBindVertexArray(0);
    }

    void setColor(const glm::vec3& newColor) {
        this->color = newColor;
        // Update the vertex buffer to reflect the color change
        this->vertices[0].color = newColor;
        updateVertexBuffer();
    }

    glm::vec3 getColor() const {
        return color;
    }

    std::type_index getType() const {
        return typeid(Point);
    }

    void update(float deltaTime) override {
        Shape::update(deltaTime);
        // No additional logic for points currently
    }
};

#endif //GAMEENGINE_POINT_H
