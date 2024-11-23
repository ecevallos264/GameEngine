#ifndef SOFTWAREENGINEERINGPROJECT_SHAPE_H
#define SOFTWAREENGINEERINGPROJECT_SHAPE_H

#include <vector>
#include <glm/vec3.hpp>
#include "shape.h"
#include "glm/ext/matrix_transform.hpp"

class Shape {
public:
    bool destroyed = false;

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    glm::vec3 position;
    glm::vec3 rotation;
    float dilation = 1.0f;

private:
    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int EBO = 0;

public:
    virtual ~Shape() {
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteVertexArrays(1, &VAO);
    }

    virtual const std::vector<float>& getVertices() const {
        return vertices;
    }

    virtual const std::vector<unsigned int>& getIndices() const {
        return indices;
    }

    glm::vec3 getPosition() const {
        return position;
    }

    unsigned int getVAO() const {
        return VAO;
    }

    unsigned int getVBO() const {
        return VBO;
    }

    unsigned int getEBO() const {
        return EBO;
    }

    void setDestroyed() {
        this->destroyed = true;
    }

    bool isDestroyed() {
        return this->destroyed;
    }

    virtual void setRotation(glm::vec3 rotation_matrix) {
        this->rotation = rotation_matrix;
    }

    float getDilation() {
        return this->dilation;
    }

    void setDilation(float factor) {
        this->dilation = factor;
    }

    void setPosition(const glm::vec3& pos) {
        position = pos;
    }

    void initializeBuffers() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0); // Position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float))); // Color
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
    }

    virtual void draw() = 0;

    virtual void setColor(glm::vec3 newColor) {
        for(int i = 0; i < this->vertices.size(); i += 7) {
            this->vertices[i + 3] = newColor.x;
            this->vertices[i + 4] = newColor.y;
            this->vertices[i + 5] = newColor.z;
        }

        glBindBuffer(GL_ARRAY_BUFFER, this->getVBO());
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data());
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    virtual void setOpacity(float opacity) {
        for(int i = 0; i < this->vertices.size(); i += 7) {
            this->vertices[i + 6] = opacity;
        }

        glBindBuffer(GL_ARRAY_BUFFER, this->getVBO());
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data());
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    virtual void update(float deltaTime, Shader* shader) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, this->getPosition());
        model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(dilation));
        shader->setMat4("model", model);
    }
};

#endif //SOFTWAREENGINEERINGPROJECT_SHAPE_H
