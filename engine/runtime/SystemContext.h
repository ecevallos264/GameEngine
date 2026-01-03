#ifndef GAMEENGINE_SYSTEM_CONTEXT_H
#define GAMEENGINE_SYSTEM_CONTEXT_H

#include <GLFW/glfw3.h>

struct SystemContext {
    GLFWwindow* window = nullptr;
    float deltaTime = 0.0f;
};

#endif //GAMEENGINE_SYSTEM_CONTEXT_H
