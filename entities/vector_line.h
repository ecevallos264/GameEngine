#ifndef SOFTWAREENGINEERINGPROJECT_VECTOR_LINE_H
#define SOFTWAREENGINEERINGPROJECT_VECTOR_LINE_H

#include "../shape.h"
#include "../shader-compiler.h"

class Vector : public Shape {
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 rotation = glm::vec3(0.0f);
    glm::vec3 color = glm::vec3(1.0f);
    float opacity = 1.0f;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

public:
    Vector(glm::vec3 position, glm::vec3 color, float opacity)
            : position(position), color(color), opacity(opacity) {
        float maxRenderDistance = 100.0f;

        // Define vertices for the vector line
        vertices = {
                0.0f, maxRenderDistance, 0.0f, color.x, color.y, color.z, opacity,  // Start point
                0.0f, -maxRenderDistance, 0.0f, color.x, color.y, color.z, opacity // End point
        };

        // Define indices for the line
        indices = { 0, 1 };

        // Initialize buffers
        initializeBuffers();
    }

    void draw() override {
        glBindVertexArray(this->getVAO());
        glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void update(float deltaTime, Shader* shader) override {
        glm::mat4 model = glm::mat4(1.0f);

        // Apply translation
        model = glm::translate(model, position);

        // Apply rotation
        model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

        // Pass the model matrix to the shader
        shader->setMat4("model", model);
    }

    void setPosition(glm::vec3 newPosition) {
        this->position = newPosition;
    }

    void setRotation(glm::vec3 newRotation) {
        this->rotation = newRotation;
    }

    void setColor(glm::vec3 newColor) {
        this->color = newColor;

        // Update color in vertex data
        vertices[3] = newColor.x;
        vertices[4] = newColor.y;
        vertices[5] = newColor.z;
        vertices[10] = newColor.x;
        vertices[11] = newColor.y;
        vertices[12] = newColor.z;

        initializeBuffers();
    }

    void setOpacity(float newOpacity) {
        this->opacity = newOpacity;

        // Update opacity in vertex data
        vertices[6] = newOpacity;
        vertices[13] = newOpacity;

        initializeBuffers();
    }
};

#endif // SOFTWAREENGINEERINGPROJECT_VECTOR_LINE_H
