#ifndef SOFTWAREENGINEERINGPROJECT_SHAPE_H
#define SOFTWAREENGINEERINGPROJECT_SHAPE_H

#include <vector>
#include "glm/vec3.hpp"

class Shape {
protected:
    glm::vec3 position;
public:
    virtual ~Shape() = default; // Virtual destructor
    virtual std::vector<float> getVertices() = 0; // Pure virtual function
    virtual std::vector<unsigned int> getIndices() = 0; // Pure virtual function
    glm::vec3 getPosition() {
        return position;
    }
};

#endif //SOFTWAREENGINEERINGPROJECT_SHAPE_H