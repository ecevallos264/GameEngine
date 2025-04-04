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

    std::vector<float> vertexBuffer;
    std::vector<unsigned int> indices;
    std::function<int(double)> onUpdate;

    glm::vec3 rotation = glm::vec3(0.0f);
    glm::vec3 scale = glm::vec3(1.0f);
    glm::mat4 model = glm::mat4(1.0f);


public:
    bool isAlive = true;

    RenderableEntity(Shader* shader) : Entity(shader) {
        //TODO implement boundingBoxLogic

    }

    virtual const std::vector<Vertex>& getVertices() const {
        return vertices;
    }

    virtual std::vector<Vertex> getTransformedVertices() const {
        std::vector<Vertex> transformedVertices;

        // Create rotation matrices for each axis (pitch, yaw, roll)
        glm::mat4 rotationX = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));  // Pitch
        glm::mat4 rotationY = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));  // Yaw
        glm::mat4 rotationZ = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));  // Roll

        // Combine rotations in ZYX order (roll -> yaw -> pitch)
        glm::mat4 rotationMatrix = rotationZ * rotationY * rotationX;

        // Create the model matrix
        glm::mat4 model = glm::translate(glm::mat4(1.0f), position) *
                          rotationMatrix *
                          glm::scale(glm::mat4(1.0f), scale);

        // Apply the model transformation to each vertex
        for (const auto& vertex : vertices) {
            glm::vec4 transformedPos = model * glm::vec4(vertex.position, 1.0f);
            transformedVertices.push_back({
                                                  {transformedPos.x, transformedPos.y, transformedPos.z},
                                                  vertex.color,
                                                  1.0f
                                          });
        }

        return transformedVertices;
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

    glm::vec3 getDilation() {
        return this->scale;
    }

    void setDilation(glm::vec3 factor) {
        this->scale = factor;
    }

    void setPosition(const glm::vec3& pos) {
        position = pos;
    }

    void setOnUpdateCallback(std::function<int(double)> callback) {
        this->onUpdate = callback;
    }

    void initializeBuffers() {
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);

            glBindVertexArray(VAO);

            // Ensure vertex data is not empty
            if (vertices.empty()) {
                std::cout << "ERROR: Vertex buffer is empty!" << std::endl;
                return;
            }

            // Bind and upload vertex data
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

            // Ensure index buffer is not empty
            if (indices.empty()) {
                std::cout << "ERROR: Index buffer is empty!" << std::endl;
                return;
            }

            // Bind and upload index data
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

            // Extract position (location 0)
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
            glEnableVertexAttribArray(0);

            // Extract color (location 1)
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
            glEnableVertexAttribArray(1);

            // Extract alpha (location 2)
            glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, alpha));
            glEnableVertexAttribArray(2);

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

    void checkOpenGLError(std::string msg) {
        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR) {
            std::cerr << "["  << err << "]: " << msg << std::endl;
        }
    }

    glm::vec3 position = glm::vec3(0.0f);
    std::vector<Vertex> vertices;
};

#endif //GAMEENGINE_RENDERABLEENTITY_H
