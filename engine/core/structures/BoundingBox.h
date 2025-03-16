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


class BoundingBox {
public:
    glm::vec3 max;
    glm::vec3 min;
    BoundingBox(glm::vec3 min, glm::vec3 max) : min(min), max(max) {
        // std::cout << "BoundingBox created with min: " << min.x << ", " << min.y << ", " << min.z << " and max: " << max.x << ", " << max.y << ", " << max.z << std::endl;
    }

    BoundingBox() : min(glm::vec3(0)), max(glm::vec3(0)) {}

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
