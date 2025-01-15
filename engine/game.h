#ifndef GAME_ENGINE_GAME_H
#define GAME_ENGINE_GAME_H

#include <glfw/glfw3.h>
#include "core/patterns/Singleton.h"
#include "core/shaders/shader-compiler.h"

class Game : public Singleton<Game> {
private:


public:
    Shader* shader;

    Game() = default;

    void run(GLFWwindow* window);

private:
    void renderGrid();
};

#endif //SOFTWAREENGINEERINGPROJECT_GAME_H
