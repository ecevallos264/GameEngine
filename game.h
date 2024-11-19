#ifndef SOFTWAREENGINEERINGPROJECT_GAME_H
#define SOFTWAREENGINEERINGPROJECT_GAME_H
#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH 800

#include <glfw/glfw3.h>
#include "shader-compiler.h"
#include "Singleton.h"
#include "shape.h"

class Game : public Singleton<Game> {
private:
    std::vector<Shape*> items;


public:
   bool shiftPressed = false;
    double lastX, lastY;
    bool firstMouse = true;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    Shader* shader;

    Game() = default;

    void run(GLFWwindow* window);
    void renderGame(GLFWwindow* window);
    static void processInput(GLFWwindow *window, float deltaTime);
    void mouse_callback(GLFWwindow* window, double xpos, double ypos);
};

#endif //SOFTWAREENGINEERINGPROJECT_GAME_H
