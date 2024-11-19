//
// Created by eceva on 11/18/2024.
//

#ifndef SOFTWAREENGINEERINGPROJECT_SHAPE_H
#define SOFTWAREENGINEERINGPROJECT_SHAPE_H

#include <vector>

virtual class Shape {
public:
    std::vectorgetVertices();

private:
    virtual std::vector<float> vertices;
    std::vector<unsigned int> indices;
};

#endif //SOFTWAREENGINEERINGPROJECT_SHAPE_H
