#ifndef GAME_ENGINE_SHAPE_H
#define GAME_ENGINE_SHAPE_H

#include <vector>
#include "glm/vec3.hpp"
#include "shape.h"
#include "glm/ext/matrix_transform.hpp"
#include "../rendering/Entity.h"
#include "Vertex.h"

class Shape : public Entity {
public:
    bool destroyed = false;

    std::vector<Vertex> vertices;
    std::vector<float> vertexBuffer;
    std::vector<unsigned int> indices;
    std::function<void(double)> onUpdate;

    glm::vec3 rotation;
    float dilation = 1.0f;

private:
    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int EBO = 0;

public:
    Shape(Shader* shader) : Entity(shader) {}

    virtual ~Shape() {
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteVertexArrays(1, &VAO);
    }

    virtual const std::vector<Vertex>& getVertices() const {
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

    void setOnUpdateCallback(std::function<void(double)> callback) {
        this->onUpdate = callback;
    }

    void initializeBuffers() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertexBuffer.size() * sizeof(float), vertexBuffer.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0); // Position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float))); // Color
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
    }

    virtual void setColor(glm::vec3 newColor) {
        for(int i = 0; i < this->vertices.size(); i++) {
            vertices[i].color = newColor;
        }

        glBindBuffer(GL_ARRAY_BUFFER, this->getVBO());
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data());
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    virtual void setOpacity(float opacity) {
        for(int i = 0; i < this->vertices.size(); i++) {
            vertices[i].alpha = opacity;
        }

        glBindBuffer(GL_ARRAY_BUFFER, this->getVBO());
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data());
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    virtual void updateVertexBuffer() {
        vertexBuffer = std::vector<float>();
        for(Vertex vertex: vertices) {
            this->vertexBuffer.push_back(vertex.position.x);
            this->vertexBuffer.push_back(vertex.position.y);
            this->vertexBuffer.push_back(vertex.position.z);
            this->vertexBuffer.push_back(vertex.color.x);
            this->vertexBuffer.push_back(vertex.color.y);
            this->vertexBuffer.push_back(vertex.color.z);
            this->vertexBuffer.push_back(vertex.alpha);
        }
    }

    glm::vec3 getSupportPoint(const glm::vec3& direction) {
        float maxDot = -std::numeric_limits<float>::infinity();
        glm::vec3 supportPoint;

        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1, 0, 0)) *
                                   glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0, 1, 0)) *
                                   glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0, 0, 1));

        for (const auto& vertex : vertices) {
            glm::vec3 transformedVertex = position + dilation * glm::vec3(rotationMatrix * glm::vec4(vertex.position, 1.0f));
            float dotProduct = glm::dot(transformedVertex, direction);
            if (dotProduct > maxDot) {
                maxDot = dotProduct;
                supportPoint = transformedVertex;
            }
        }

        return supportPoint;
    }
};

#endif //SOFTWAREENGINEERINGPROJECT_SHAPE_H
