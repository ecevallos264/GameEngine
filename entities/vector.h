#ifndef GAME_ENGINE_VECTOR_H
#define GAME_ENGINE_VECTOR_H

#include "../shape.h"
#include "../shader-compiler.h"
#include "../Singleton.h"
#include "../settings.h"

class Vector : public Shape {
    glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f); // Rotation angles (Euler angles)

public:
    Vector(const glm::vec3& origin, glm::vec3 color)
    {
        glm::vec3 startpoint = origin;
        glm::vec3 endpoint = origin + glm::vec3(0.0f, Settings::getInstance().MAX_RENDER_DISTANCE, 0.0f);

        std::cout << startpoint.x << ',' << startpoint.y << ',' << startpoint.z << std::endl;
        vertices = {
                startpoint.x, startpoint.y, startpoint.z,   color.x, color.y, color.z,      1.0, // Start point
                endpoint.x, endpoint.y, endpoint.z,         color.x, color.y, color.z,      1.0, // End point
        };
        indices = { 0, 1 };
        initializeBuffers();
    }

    void setRotation(glm::vec3 newRotation) override {
        rotation = newRotation;
        updateVertices();
    }

    void draw() override {
        glBindVertexArray(this->getVAO());
        glDrawElements(GL_LINES, this->getIndices().size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

private:
    void updateVertices() {
        // Extract the origin of the vector
        glm::vec3 origin(vertices[0], vertices[1], vertices[2]);

        // Define the endpoint relative to the origin
        glm::vec3 endpoint = origin + glm::vec3(0.0f, Settings::getInstance().MAX_RENDER_DISTANCE, 0.0f);

        // Define the rotation matrix
        glm::mat4 rotationMatrix(1.0f);
        rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation.x), glm::vec3(1, 0, 0));
        rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation.y), glm::vec3(0, 1, 0));
        rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation.z), glm::vec3(0, 0, 1));
        glm::vec4 translatedOrigin = glm::vec4(origin, 1.0f) - glm::vec4(origin, 0.0f);
        glm::vec4 translatedEndpoint = glm::vec4(endpoint, 1.0f) - glm::vec4(origin, 0.0f);

        translatedOrigin = rotationMatrix * translatedOrigin;
        translatedEndpoint = rotationMatrix * translatedEndpoint;

        translatedOrigin += glm::vec4(origin, 0.0f);
        translatedEndpoint += glm::vec4(origin, 0.0f);

        vertices[0] = translatedOrigin.x;
        vertices[1] = translatedOrigin.y;
        vertices[2] = translatedOrigin.z;
        vertices[7] = translatedEndpoint.x;
        vertices[8] = translatedEndpoint.y;
        vertices[9] = translatedEndpoint.z;

        glBindBuffer(GL_ARRAY_BUFFER, this->getVBO());
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data());
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
};

#endif // SOFTWAREENGINEERINGPROJECT_VECTOR_H