//
// Created by eceva on 1/29/2025.
//

#ifndef GAMEENGINE_BOUNDINGREGION_H
#define GAMEENGINE_BOUNDINGREGION_H

#include "Entity.h"
#include "RenderableEntity.h"
#include "../core/settings/settings.h"

class BoundingRegion : public RenderableEntity {
private:
    glm::vec3 color;
    float alpha;

    double minX, maxX;
    double minY, maxY;
    double minZ, maxZ;

    bool minXInitialized = false;
    bool maxXInitialized = false;
    bool minYInitialized = false;
    bool maxYInitialized = false;
    bool minZInitialized = false;
    bool maxZInitialized = false;

public:
    BoundingRegion(Shader* shader, const std::vector<Vertex>& vertexList, glm::mat4 modelMatrix, float alpha)
            : RenderableEntity(shader), color(Settings::BOUNDING_REGION_COLOR), alpha(alpha),
              minX(DBL_MAX), maxX(DBL_MIN), minY(DBL_MAX), maxY(DBL_MIN), minZ(DBL_MAX), maxZ(DBL_MIN) {

        if (vertexList.empty()) {
            return;
        }

        // Apply transformation to each vertex and find min/max in world space
        for (const auto& vertex : vertexList) {
            glm::vec4 transformedPos = modelMatrix * glm::vec4(vertex.position, 1.0f);

            if (!minXInitialized || transformedPos.x < minX) {
                minX = transformedPos.x;
                minXInitialized = true;
            }
            if (!maxXInitialized || transformedPos.x > maxX) {
                maxX = transformedPos.x;
                maxXInitialized = true;
            }

            if (!minYInitialized || transformedPos.y < minY) {
                minY = transformedPos.y;
                minYInitialized = true;
            }
            if (!maxYInitialized || transformedPos.y > maxY) {
                maxY = transformedPos.y;
                maxYInitialized = true;
            }

            if (!minZInitialized || transformedPos.z < minZ) {
                minZ = transformedPos.z;
                minZInitialized = true;
            }
            if (!maxZInitialized || transformedPos.z > maxZ) {
                maxZ = transformedPos.z;
                maxZInitialized = true;
            }
        }

        // Define transformed bounding box vertices
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


    glm::vec3 getCenter() const {
        return glm::vec3((minX + maxX) * 0.5f, (minY + maxY) * 0.5f, (minZ + maxZ) * 0.5f);
    }

    glm::vec3 getSize() const {
        return glm::vec3(abs(maxX - minX), abs(maxY - minY), abs(maxZ - minZ));
    }

    void render(glm::mat4 view, glm::mat4 projection) override {
        shader->use();

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glLineWidth(2.0f);

        model = glm::translate(model, position);
        model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0));
        model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0, 1, 0));
        model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1));
        model = glm::scale(model, scale);


        shader->setMat4("view", view);
        shader->setMat4("projection", projection);
        shader->setMat4("model", model);
        shader->setVec3("shapeColor", color);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glBindVertexArray(VAO);

        glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    void update(float deltaTime) override {
        initializeBuffers();
    }

    ~BoundingRegion() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
};

#endif // GAMEENGINE_BOUNDINGREGION_H
