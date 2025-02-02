#ifndef GAME_ENGINE_SHAPE_H
#define GAME_ENGINE_SHAPE_H

#include <vector>
#include "glm/vec3.hpp"
#include "Shape.h"
#include "glm/ext/matrix_transform.hpp"
#include "../rendering/Entity.h"
#include "Vertex.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
#include "../rendering/BoundingRegion.h"
#include "../rendering/RenderableEntity.h"

//TODO rename to CollidableEntity or similar
class Shape : public RenderableEntity {
public:
    bool destroyed = false;

    BoundingRegion* boundingRegion;
public:
    Shape(Shader* shader) : RenderableEntity(shader) {}

    void setDestroyed() {
        this->destroyed = true;
    }

    bool isDestroyed() {
        return this->destroyed;
    }

    glm::vec3 getSupportPoint(const glm::vec3& direction) {
        if (glm::length(direction) < 1e-6f) {
            std::cerr << "Direction vector is too small: " << glm::to_string(direction) << std::endl;
            throw std::invalid_argument("Direction vector is too small.");
        }

        float maxDot = -std::numeric_limits<float>::infinity();
        glm::vec3 supportPoint;

        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1, 0, 0)) *
                                   glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0, 1, 0)) *
                                   glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0, 0, 1));

        for (const auto& vertex : vertices) {
            glm::vec3 scaledVertex = scale * vertex.position;
            glm::vec3 transformedVertex = position + glm::vec3(rotationMatrix * glm::vec4(scaledVertex, 1.0f));

            float dotProduct = glm::dot(transformedVertex, direction);
            if (dotProduct > maxDot) {
                maxDot = dotProduct;
                supportPoint = transformedVertex;
            }

        }

        return supportPoint;
    }

    void updateBoundingRegion(glm::mat4 model) {
        this->boundingRegion = new BoundingRegion(this->shader, this->vertices, model, 1.0f);
    }

    void setRenderBoundingRegionVisibility(bool state) {
        this->regionBoundingVisible = state;
    }

    bool regionBoundingVisible;
};

#endif //SOFTWAREENGINEERINGPROJECT_SHAPE_H
