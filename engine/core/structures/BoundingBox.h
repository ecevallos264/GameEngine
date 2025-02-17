//
// Created by eceva on 2/5/2025.
//

#ifndef GAMEENGINE_BOUNDINGBOX_H
#define GAMEENGINE_BOUNDINGBOX_H
#include <glm/vec3.hpp>
#include "../../entitities/Vertex.h"
#include "glm/common.hpp"
#include "glm/glm.hpp"
#include "../../rendering/RenderableEntity.h"


class BoundingBox : public RenderableEntity {
public:
    Shader* shader;
    glm::vec3 max;
    glm::vec3 min;
    BoundingBox(Shader* shader, glm::vec3 min, glm::vec3 max, glm::mat4 modelMatrix, float alpha) : RenderableEntity(shader), shader(shader) {}

//    BoundingBox() {}

    bool contains(BoundingBox* box) {
        return (box->min.x >= min.x && box->max.x <= max.x &&
                box->min.z >= min.z && box->max.z <= max.z);
    }

    static BoundingBox* fromVertices(Shader* shader, std::vector<Vertex>& vertices) {
        glm::vec3 min;
        glm::vec3 max;

        for (const auto& vertex : vertices) {
            min = glm::min(min, vertex.position);
            max = glm::max(max, vertex.position);
        }
        return new BoundingBox(shader, min, max, glm::mat4(1.0f), 1.0f);
    }

    static BoundingBox* fromPoints(Shader* shader, std::vector<glm::vec3> points) {
        glm::vec3 min;
        glm::vec3 max;
        for (const auto& point : points) {
            min = glm::min(min, point);
            max = glm::max(max, point);
        }
        return new BoundingBox(nullptr, min, max, glm::mat4(1.0f), 1.0f);
    }
};

void renderBoundingBox(BoundingBox* box, glm::mat4 view, glm::mat4 projection) {

}


#endif //GAMEENGINE_BOUNDINGBOX_H
