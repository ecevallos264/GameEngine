#ifndef SOFTWAREENGINEERINGPROJECT_VECTOR_H
#define SOFTWAREENGINEERINGPROJECT_VECTOR_H

#include "../shape.h"
#include "../shader-compiler.h"

class Vector : public Shape {
    glm::vec3 direction = glm::vec3(0.0f, 1.0f, 0.0f); // Default direction
    float maxDistance = 100.0f; // Maximum rendering distance
    glm::vec3 rotation = glm::vec3(0.0f); // Rotation angles (Euler angles)

public:
    Vector(const glm::vec3& origin, const glm::vec3& direction,
           float maxDistance, glm::vec3 color, float opacity)
            : direction(glm::normalize(direction)), maxDistance(maxDistance)
    {
        glm::vec3 endpoint = origin + this->direction * this->maxDistance;

        vertices = {
                origin.x, origin.y, origin.z, color.x, color.y, color.z, opacity, // Start point
                endpoint.x, endpoint.y, endpoint.z, color.x, color.y, color.z, opacity, // End point
        };
        indices = { 0, 1 };
        initializeBuffers();
    }

    void setDirection(const glm::vec3& newDirection) {
        direction = glm::normalize(newDirection);
        updateVertices();
    }

    void setRotation(const glm::vec3& newRotation) {
        rotation = newRotation;
        updateVertices();
    }

    void setMaxDistance(float distance) {
        maxDistance = distance;
        updateVertices();
    }

    void update(float deltaTime, Shader* shader) override {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, this->position);
        shader->setMat4("model", model);
        updateVertices();
    }

    void draw() override {
        glBindVertexArray(this->getVAO());
        glDrawElements(GL_LINES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

private:
    void updateVertices() {
        glm::vec3 origin(vertices[0], vertices[1], vertices[2]);
        glm::vec3 endpoint = origin + direction * maxDistance;

        // Update vertex positions
        vertices[7] = endpoint.x;
        vertices[8] = endpoint.y;
        vertices[9] = endpoint.z;

        // Update buffer data
        glBindBuffer(GL_ARRAY_BUFFER, this->getVBO());
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data());
    }
};

#endif // SOFTWAREENGINEERINGPROJECT_VECTOR_H
