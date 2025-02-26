//
// Created by eceva on 2/5/2025.
//

#ifndef GAMEENGINE_BOUNDINGBOX_H
#define GAMEENGINE_BOUNDINGBOX_H
#include <glm/vec3.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "../../entitities/Vertex.h"
#include "glm/common.hpp"
#include "glm/glm.hpp"
#include "../../rendering/RenderableEntity.h"
#include "../shaders/ShaderManager.h"


class BoundingBox : public RenderableEntity {
public:
    glm::vec3 max;
    glm::vec3 min;
    BoundingBox(glm::vec3 min, glm::vec3 max) : RenderableEntity(ShaderManager::getInstance().getShader("shader1")), min(min), max(max) {
        // std::cout << "BoundingBox created with min: " << min.x << ", " << min.y << ", " << min.z << " and max: " << max.x << ", " << max.y << ", " << max.z << std::endl;
        vertices = {
            Vertex(glm::vec3(min.x, min.y, min.z), color, 1.0f), // Bottom-left-back
            Vertex(glm::vec3(max.x, min.y, min.z), color, 1.0f), // Bottom-right-back
            Vertex(glm::vec3(min.x, max.y, min.z), color, 1.0f), // Top-left-back
            Vertex(glm::vec3(max.x, max.y, min.z), color, 1.0f), // Top-right-back
            Vertex(glm::vec3(min.x, min.y, max.z), color, 1.0f), // Bottom-left-front
            Vertex(glm::vec3(max.x, min.y, max.z), color, 1.0f), // Bottom-right-front
            Vertex(glm::vec3(min.x, max.y, max.z), color, 1.0f), // Top-left-front
            Vertex(glm::vec3(max.x, max.y, max.z), color, 1.0f)  // Top-right-front
    };

        // Define indices for wireframe rendering
        indices = {
            0, 1,  1, 3,  3, 2,  2, 0,  // Back face
            4, 5,  5, 7,  7, 6,  6, 4,  // Front face
            0, 4,  1, 5,  3, 7,  2, 6   // Connecting edges
    };

        initializeBuffers();
    }

    BoundingBox() : RenderableEntity(ShaderManager::getInstance().getShader("shader1")), min(glm::vec3(0)), max(glm::vec3(0)) {
        vertices = {
                Vertex(glm::vec3(min.x, min.y, min.z), color, 1.0f), // Bottom-left-back
                Vertex(glm::vec3(max.x, min.y, min.z), color, 1.0f), // Bottom-right-back
                Vertex(glm::vec3(min.x, max.y, min.z), color, 1.0f), // Top-left-back
                Vertex(glm::vec3(max.x, max.y, min.z), color, 1.0f), // Top-right-back
                Vertex(glm::vec3(min.x, min.y, max.z), color, 1.0f), // Bottom-left-front
                Vertex(glm::vec3(max.x, min.y, max.z), color, 1.0f), // Bottom-right-front
                Vertex(glm::vec3(min.x, max.y, max.z), color, 1.0f), // Top-left-front
                Vertex(glm::vec3(max.x, max.y, max.z), color, 1.0f)  // Top-right-front
        };

        indices = {
                0, 1,  1, 3,  3, 2,  2, 0,  // Back face
                4, 5,  5, 7,  7, 6,  6, 4,  // Front face
                0, 4,  1, 5,  3, 7,  2, 6   // Connecting edges
        };

        initializeBuffers();
    }

    bool contains(BoundingBox* box) {
        return (box->min.x >= min.x && box->max.x <= max.x &&
                box->min.z >= min.z && box->max.z <= max.z);
    }

    static BoundingBox fromVertices(std::vector<Vertex>& vertices) {
        glm::vec3 min;
        glm::vec3 max;

        for (const auto& vertex : vertices) {
            min = glm::min(min, vertex.position);
            max = glm::max(max, vertex.position);
        }
        return BoundingBox(min, max);
    }

    static BoundingBox* fromPoints(std::vector<glm::vec3> points) {
        glm::vec3 min;
        glm::vec3 max;

        // Initialize min and max to the first point
        if (!points.empty()) {
            min = points[0];
            max = points[0];
        } // If points is empty, return a default bounding box
        else {
            return new BoundingBox();
        }
        for (const auto& point : points) {
            min = glm::min(min, point);
            max = glm::max(max, point);
        }
        return new BoundingBox(min, max);
    }

    static BoundingBox* combine(BoundingBox* boundingBox1, BoundingBox* boundingBox2) {
        return new BoundingBox(glm::min(boundingBox1->min, boundingBox2->min), glm::max(boundingBox1->max, boundingBox2->max));
    }

    void render(glm::mat4 view, glm::mat4 projection) override {
        // std::cout << "Rendering bounding box dimensions: " << calculateDimensions().x << ":" << calculateDimensions().y << ":" << calculateDimensions().z << std::endl;        shader->use();

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
        shader->setVec3("shapeColor", glm::vec3(1.0f, 1.0, 1.0));

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glBindVertexArray(VAO);

        glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    void setColor(glm::vec3 color) {
        for (Vertex& vertex : vertices) {
            vertex.color = color;
        }
        updateVertexBuffer();
        initializeBuffers();
    }



    glm::vec3 calculateDimensions() {
        return max - min;
    }

    glm::vec3 calculateCenter() {
        return (min + max) / 2.0f;
    }

    bool operator==(BoundingBox br) {
        return min == br.min && max == br.max;
    }
};
#endif //GAMEENGINE_BOUNDINGBOX_H
