#ifndef SOFTWAREENGINEERINGPROJECT_SHAPE_H
#define SOFTWAREENGINEERINGPROJECT_SHAPE_H

#include <vector>
#include <glm/vec3.hpp>
#include "shape.h"
#include "glm/ext/matrix_transform.hpp"

class Shape {
protected:
    bool destroyed = false;
    glm::vec3 position;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    float scaleFactor = 1.0f;

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

    void setVAO(unsigned int vao) {
        this->VAO = vao;
    }

    void setVBO(unsigned int vbo) {
        this->VBO = vbo;
    }

    void setEBO(unsigned int ebo) {
        this->EBO = ebo;
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

    float getScaleFactor() const {
        return this->scaleFactor;
    }

    void setScaleFactor(float factor) {
        this->scaleFactor = factor;
    }

    void setDestroyed() {
        this->destroyed = true;
    }

    bool isDestroyed() {
        return this->destroyed;
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

    void setPosition(const glm::vec3& pos) {
        position = pos;
    }

    virtual void draw() {
        glBindVertexArray(this->getVAO());
        glDrawElements(GL_TRIANGLES, this->getIndices().size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    virtual void update(float deltaTime, Shader* shader) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, this->getPosition());
        model = glm::scale(model, glm::vec3(1.0f, ((float)WINDOW_WIDTH / (float)WINDOW_HEIGHT) * scaleFactor, 1.0f));
        shader->setMat4("model", model);
    }
};

#endif //SOFTWAREENGINEERINGPROJECT_SHAPE_H
