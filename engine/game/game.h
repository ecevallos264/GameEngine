#ifndef GAME_ENGINE_GAME_H
#define GAME_ENGINE_GAME_H

#include <glfw/glfw3.h>
#include "../utils/patterns/Singleton.h"
#include "../utils/shaders/shader-compiler.h"

class Game : public Singleton<Game> {
private:


public:
    Shader* shader;

    Game() = default;

    void run(GLFWwindow* window);
    void renderGame(GLFWwindow* window);
    void processInput(GLFWwindow *window, float deltaTime);
    void mouse_callback(GLFWwindow* window, double xpos, double ypos);

private:
    void renderGrid();
};

#endif //SOFTWAREENGINEERINGPROJECT_GAME_H
