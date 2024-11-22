#ifndef SOFTWAREENGINEERINGPROJECT_VECTOR_H
#define SOFTWAREENGINEERINGPROJECT_VECTOR_H

#include "../shape.h"
#include "../shader-compiler.h"
#include "../Singleton.h"
#include "../settings.h"

class Vector : public Shape {
    glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f); // Rotation angles (Euler angles)

public:
    Vector(const glm::vec3& origin, glm::vec3 color, float opacity)
    {
        glm::vec3 startpoint = origin;
        glm::vec3 endpoint = origin + glm::vec3(0.0f, Settings::getInstance().MAX_RENDER_DISTANCE, 0.0f);

        std::cout << startpoint.x << ',' << startpoint.y << ',' << startpoint.z << std::endl;
        vertices = {
                startpoint.x, startpoint.y, startpoint.z,   color.x, color.y, color.z,      opacity, // Start point
                endpoint.x, endpoint.y, endpoint.z,         color.x, color.y, color.z,      opacity, // End point
        };
        indices = { 0, 1 };
        initializeBuffers();
    }

    void setRotation(const glm::vec3& newRotation) {
        rotation = newRotation;
        updateVertices();
    }

    void update(float deltaTime, Shader* shader) override {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, this->getPosition());
        model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        shader->setMat4("model", model);
    }

    void draw() override {
        glBindVertexArray(this->getVAO());
        glDrawElements(GL_LINES, this->getIndices().size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

private:
    void updateVertices() {
        glm::vec3 origin(vertices[0], vertices[1], vertices[2]);
        glm::vec3 endpoint = origin + glm::vec3(0.0f, Settings::getInstance().MAX_RENDER_DISTANCE, 0.0f);

        // Update vertex positions
        vertices[7] = endpoint.x;
        vertices[8] = endpoint.y;
        vertices[9] = endpoint.z;

        // Update buffer data
        glBindBuffer(GL_ARRAY_BUFFER, this->getVBO());
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data());
        glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind buffer after updating
    }
};

#endif // SOFTWAREENGINEERINGPROJECT_VECTOR_H