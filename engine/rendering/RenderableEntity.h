//
// Created by eceva on 1/30/2025.
//

#ifndef GAMEENGINE_RENDERABLEENTITY_H
#define GAMEENGINE_RENDERABLEENTITY_H

#include "Entity.h"
#include "../entitities/Vertex.h"
#include "../core/shaders/shader-compiler.h"

/*
 * Should be able to render the entity without anything else
 * */
class RenderableEntity : public Entity {
protected:
    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int EBO = 0;

    std::vector<Vertex> vertices;
    std::vector<float> vertexBuffer;
    std::vector<unsigned int> indices;
    std::function<void(double)> onUpdate;

    // Transformations
    glm::vec3 rotation;
    float dilation = 1.0f;


public:
    RenderableEntity(Shader* shader) : Entity(shader) {}

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

        updateVertexBuffer();

        glBindBuffer(GL_ARRAY_BUFFER, this->getVBO());
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertexBuffer.size() * sizeof(float), vertexBuffer.data());
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    virtual void setOpacity(float opacity) {
        for(int i = 0; i < this->vertices.size(); i++) {
            vertices[i].alpha = opacity;
        }

        updateVertexBuffer();

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

    virtual ~RenderableEntity() {
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteVertexArrays(1, &VAO);
    }
};

#endif //GAMEENGINE_RENDERABLEENTITY_H
