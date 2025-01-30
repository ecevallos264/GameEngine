//
// Created by eceva on 1/29/2025.
//

#ifndef GAMEENGINE_BOUNDINGREGION_H
#define GAMEENGINE_BOUNDINGREGION_H

#include "Entity.h"
#include "RenderableEntity.h"

struct BoundingRegionVertexData {
    double minX;
    double maxX;
    double minY;
    double maxY;
    double minZ;
    double maxZ;

    BoundingRegionVertexData()
            : minX(), maxX(), minY(), maxY(), minZ(), maxZ() {}

    std::vector<Vertex> getVertices(const glm::vec3& color, float alpha) {
        glm::vec3 minCorner = glm::vec3(minX, minY, minZ);
        glm::vec3 maxCorner = glm::vec3(maxX, maxY, maxZ);

        return {
                Vertex(glm::vec3(minCorner.x, minCorner.y, minCorner.z), color, alpha), // Bottom-left-back
                Vertex(glm::vec3(maxCorner.x, minCorner.y, minCorner.z), color, alpha), // Bottom-right-back
                Vertex(glm::vec3(minCorner.x, maxCorner.y, minCorner.z), color, alpha), // Top-left-back
                Vertex(glm::vec3(maxCorner.x, maxCorner.y, minCorner.z), color, alpha), // Top-right-back
                Vertex(glm::vec3(minCorner.x, minCorner.y, maxCorner.z), color, alpha), // Bottom-left-front
                Vertex(glm::vec3(maxCorner.x, minCorner.y, maxCorner.z), color, alpha), // Bottom-right-front
                Vertex(glm::vec3(minCorner.x, maxCorner.y, maxCorner.z), color, alpha), // Top-left-front
                Vertex(glm::vec3(maxCorner.x, maxCorner.y, maxCorner.z), color, alpha)  // Top-right-front
        };
    }

    glm::vec3 getCenter() {
        glm::vec3 minCorner = glm::vec3(minX, minY, minZ);
        glm::vec3 maxCorner = glm::vec3(maxX, maxY, maxZ);
        return (minCorner + maxCorner) * 0.5f;
    }

    glm::vec3 getSize() {
        return glm::vec3(abs(minX - maxX), abs(minY - maxY), abs(minZ - maxZ));
    }
};

class BoundingRegion : public RenderableEntity {
private:
    Shader* shader;
    BoundingRegionVertexData data;
    glm::vec3 color;
    float alpha;
public:
    BoundingRegion() : RenderableEntity(nullptr), data(), color(0.0f), alpha(0.0f) {}
//    BoundingRegion(Shader* shader, BoundingRegionVertexData data, glm::vec3 color, float alpha) : Rectangle(shader, data.getCenter(), data.getSize(), color), data(data) {
//
//    }
    BoundingRegion(Shader* shader, BoundingRegionVertexData data, glm::vec3 color, float alpha) : RenderableEntity(shader), data(data), color(color), alpha(alpha) {}

    static BoundingRegion generateBoundingRegion(const std::vector<Vertex>& vertices) {
        if (vertices.empty()) {
            throw std::runtime_error("Cannot generate a bounding region from an empty vertex list.");
        }

        BoundingRegionVertexData regionData;


        for (const auto& vertex : vertices) {
            if (vertex.position.x < regionData.minX) regionData.minX = vertex.position.x;
            if (vertex.position.x > regionData.maxX) regionData.maxX = vertex.position.x;

            if (vertex.position.y < regionData.minY) regionData.minY = vertex.position.y;
            if (vertex.position.y > regionData.maxY) regionData.maxY = vertex.position.y;

            if (vertex.position.z < regionData.minZ) regionData.minZ = vertex.position.z;
            if (vertex.position.z > regionData.maxZ) regionData.maxZ = vertex.position.z;
        }

        return BoundingRegion(nullptr, regionData, glm::vec3(1.0f, 0.0f, 0.0f), 1.0f);
    }

    glm::vec3 operator-(const BoundingRegion& other) const {
        return this->getCenter() - other.getCenter();
    }

    glm::vec3 getCenter() const {
        glm::vec3 minCorner = glm::vec3(data.minX, data.minY, data.minZ);
        glm::vec3 maxCorner = glm::vec3(data.maxX, data.maxY, data.maxZ);
        return (minCorner + maxCorner) * 0.5f;
    }

    void render(glm::mat4 view, glm::mat4 projection) override {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, data.getCenter());

        this->shader->use();
        this->shader->setMat4("view", view);
        this->shader->setMat4("projection", projection);
        this->shader->setMat4("model", model);
        this->shader->setVec3("shapeColor", glm::vec3(1.0f, 0.0f, 0.0f));

        glBindVertexArray(this->getVAO());
        glDrawElements(GL_TRIANGLES, data.getVertices(color, alpha).size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    ~BoundingRegion() {
//        vertices.clear();
//        vertexBuffer.clear();
//        indices.clear();
    }
};

#endif //GAMEENGINE_BOUNDINGREGION_H
