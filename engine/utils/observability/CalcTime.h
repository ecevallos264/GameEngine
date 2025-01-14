//
// Created by eceva on 1/14/2025.
//

#ifndef GAMEENGINE_CALCTIME_H
#define GAMEENGINE_CALCTIME_H

#include <functional>
#include <iostream>
#include <glfw/glfw3.h>

extern float calcTime(std::function<void()> callback) {
    float initTime = static_cast<float>(glfwGetTime());
    callback();
    float endTime = static_cast<float>(glfwGetTime());
    float elapsedTime = endTime - initTime;
    return elapsedTime;
}
#endif //GAMEENGINE_CALCTIME_H
