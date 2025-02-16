//
// Created by eceva on 2/5/2025.
//

#ifndef GAMEENGINE_BOUNDINGBOX_H
#define GAMEENGINE_BOUNDINGBOX_H
#include <glm/vec3.hpp>


class BoundingBox {
public:
    glm::vec3 max;
    glm::vec3 min;
    BoundingBox(glm::vec3 point1, glm::vec3 point2) {}

    BoundingBox() {}

    bool contains(BoundingBox box) {
        return (box.min.x >= min.x && box.max.x <= max.x &&
                box.min.z >= min.z && box.max.z <= max.z);
    }

};


#endif //GAMEENGINE_BOUNDINGBOX_H
