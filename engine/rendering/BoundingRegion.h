//
// Created by eceva on 1/29/2025.
//

#ifndef GAMEENGINE_BOUNDINGREGION_H
#define GAMEENGINE_BOUNDINGREGION_H

#include "Entity.h"
#include "RenderableEntity.h"
#include "../core/settings/settings.h"
#include "../core/structures/List.h"

struct BoundingRegionVertexData {
    double x1, x2;
    double y1, y2;
    double z1, z2;
};

class BoundingRegion : public RenderableEntity {
private:
    glm::vec3 color;
    float alpha;
    BoundingRegionVertexData vertexData;

    BoundingRegion* octants[8];
    List<BoundingRegion*> objects;
    char activeOctants = 0x00;


public:
    BoundingRegion(Shader* shader, BoundingRegionVertexData data, glm::mat4 modelMatrix, float alpha)
            : RenderableEntity(shader), color(Settings::BOUNDING_REGION_COLOR), alpha(alpha), vertexData(data) {
        vertices = {
                Vertex(glm::vec3(data.x1, data.y1, data.z1), color, alpha), // Bottom-left-back
                Vertex(glm::vec3(data.x2, data.y1, data.z1), color, alpha), // Bottom-right-back
                Vertex(glm::vec3(data.x1, data.y2, data.z1), color, alpha), // Top-left-back
                Vertex(glm::vec3(data.x2, data.y2, data.z1), color, alpha), // Top-right-back
                Vertex(glm::vec3(data.x1, data.y1, data.z2), color, alpha), // Bottom-left-front
                Vertex(glm::vec3(data.x2, data.y1, data.z2), color, alpha), // Bottom-right-front
                Vertex(glm::vec3(data.x1, data.y2, data.z2), color, alpha), // Top-left-front
                Vertex(glm::vec3(data.x2, data.y2, data.z2), color, alpha)  // Top-right-front
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
        return glm::vec3((vertexData.x1 + vertexData.x2) * 0.5f, (vertexData.y1 + vertexData.y2) * 0.5f, (vertexData.z1 + vertexData.z2) * 0.5f);
    }

    glm::vec3 getSize() const {
        return glm::vec3(abs(vertexData.x2 - vertexData.x1), abs(vertexData.y2 - vertexData.y1), abs(vertexData.z2 - vertexData.z1));
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

    ~BoundingRegion() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
};

#endif // GAMEENGINE_BOUNDINGREGION_H
