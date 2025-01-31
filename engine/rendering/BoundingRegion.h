//
// Created by eceva on 1/29/2025.
//

#ifndef GAMEENGINE_BOUNDINGREGION_H
#define GAMEENGINE_BOUNDINGREGION_H

#include "Entity.h"
#include "RenderableEntity.h"

class BoundingRegion : public RenderableEntity {
private:
    Shader* shader;
    glm::vec3 color;
    glm::vec3 position;
    float alpha;

    double minX, maxX;
    double minY, maxY;
    double minZ, maxZ;

    GLuint VAO, VBO, EBO;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

public:
    BoundingRegion(Shader* shader, const std::vector<Vertex>& vertexList, glm::vec3 position, glm::vec3 color, float alpha)
            : RenderableEntity(shader), shader(shader), position(position), color(color), alpha(alpha),
              minX(DBL_MAX), maxX(DBL_MIN), minY(DBL_MAX), maxY(DBL_MIN), minZ(DBL_MAX), maxZ(DBL_MIN) {

        if (vertexList.empty()) {
            std::cout << "Warning: BoundingRegion created with an empty vertex list." << std::endl;
            return;
        }

        // Calculate min/max bounds
        for (const auto& vertex : vertexList) {
            minX = std::min(minX, (double)vertex.position.x);
            maxX = std::max(maxX, (double)vertex.position.x);
            minY = std::min(minY, (double)vertex.position.y);
            maxY = std::max(maxY, (double)vertex.position.y);
            minZ = std::min(minZ, (double)vertex.position.z);
            maxZ = std::max(maxZ, (double)vertex.position.z);
        }

        // Define vertices
        vertices = {
                Vertex(glm::vec3(minX, minY, minZ), color, alpha), // Bottom-left-back
                Vertex(glm::vec3(maxX, minY, minZ), color, alpha), // Bottom-right-back
                Vertex(glm::vec3(minX, maxY, minZ), color, alpha), // Top-left-back
                Vertex(glm::vec3(maxX, maxY, minZ), color, alpha), // Top-right-back
                Vertex(glm::vec3(minX, minY, maxZ), color, alpha), // Bottom-left-front
                Vertex(glm::vec3(maxX, minY, maxZ), color, alpha), // Bottom-right-front
                Vertex(glm::vec3(minX, maxY, maxZ), color, alpha), // Top-left-front
                Vertex(glm::vec3(maxX, maxY, maxZ), color, alpha)  // Top-right-front
        };

        // Define indices for wireframe rendering
        indices = {
                0, 1,  1, 3,  3, 2,  2, 0,  // Back face
                4, 5,  5, 7,  7, 6,  6, 4,  // Front face
                0, 4,  1, 5,  3, 7,  2, 6   // Connecting edges
        };

        initializeBuffers();
    }

    static BoundingRegion generateBoundingRegion(Shader* shader, const std::vector<Vertex>& vertices, glm::vec3 pos) {
        return BoundingRegion(shader, vertices, pos, glm::vec3(1.0f, 0.0f, 0.0f), 1.0f);
    }

    glm::vec3 getCenter() const {
        return glm::vec3((minX + maxX) * 0.5f, (minY + maxY) * 0.5f, (minZ + maxZ) * 0.5f);
    }

    glm::vec3 getSize() const {
        return glm::vec3(abs(maxX - minX), abs(maxY - minY), abs(maxZ - minZ));
    }

    void render(glm::mat4 view, glm::mat4 projection) override {
        if (!shader || VAO == 0) return;

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glLineWidth(2.0f);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);

        shader->use();
        shader->setMat4("view", view);
        shader->setMat4("projection", projection);
        shader->setMat4("model", model);
        shader->setVec3("shapeColor", color);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glBindVertexArray(VAO);
        glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR) {
            std::cout << "OpenGL Error: " << err << std::endl;
        }
    }

    void initializeBuffers() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        // Upload vertex data
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

        // Upload index data
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        // Define vertex attributes
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
    }

    ~BoundingRegion() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
};

#endif // GAMEENGINE_BOUNDINGREGION_H
