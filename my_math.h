//
// Created by eceva on 11/19/2024.
//

#ifndef SOFTWAREENGINEERINGPROJECT_MY_MATH_H
#define SOFTWAREENGINEERINGPROJECT_MY_MATH_H

#include <glm/vec3.hpp>
#include <random>

glm::vec3 generateRandomLocation(float minX, float maxX, float minY, float maxY, float minZ, float maxZ) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> disX(minX, maxX);
    std::uniform_real_distribution<float> disY(minY, maxY);
    std::uniform_real_distribution<float> disZ(minZ, maxZ);

    float x = disX(gen);
    float y = disY(gen);
    float z = disZ(gen);

    return glm::vec3(x, y, z);
}

glm::vec3 generateRandomColor() {

}

#endif //SOFTWAREENGINEERINGPROJECT_MY_MATH_H
