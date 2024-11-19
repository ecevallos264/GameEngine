#ifndef SOFTWAREENGINEERINGPROJECT_GAME_H
#define SOFTWAREENGINEERINGPROJECT_GAME_H
#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH 800

#include <glfw/glfw3.h>
#include "shader-compiler.h"
#include "Singleton.h"

class Game : public Singleton<Game> {
public:
    Game() = default;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    unsigned int VAO;
    Shader* shader;
    void setVertices(const std::vector<float>& v);
    void setIndices(const std::vector<unsigned int>& v);
    void run(GLFWwindow* window);
    void renderGame(GLFWwindow* window);
    static void processInput(GLFWwindow *window, float deltaTime);
};

#endif //SOFTWAREENGINEERINGPROJECT_GAME_H
