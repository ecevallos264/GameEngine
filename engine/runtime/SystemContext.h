#ifndef GAMEENGINE_SYSTEM_CONTEXT_H
#define GAMEENGINE_SYSTEM_CONTEXT_H

class Window;

struct SystemContext {
    Window* window = nullptr;
    float deltaTime = 0.0f;
};

#endif //GAMEENGINE_SYSTEM_CONTEXT_H
