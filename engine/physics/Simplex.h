//
// Created by eceva on 1/15/2025.
//

#ifndef GAMEENGINE_SIMPLEX_H
#define GAMEENGINE_SIMPLEX_H

#include <vector>
#include "glm/vec3.hpp"

struct Simplex {
    std::vector<glm::vec3> points;

    void addPoint(const glm::vec3& point) {
        points.push_back(point);
    }

    glm::vec3 getLastPoint() const {
        return points.back();
    }

    void removePoint(size_t index) {
        points.erase(points.begin() + index);
    }

    size_t size() const {
        return points.size();
    }
};



#endif //GAMEENGINE_SIMPLEX_H
