#include <iostream>
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <filesystem>
#include "shader-compiler.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include "camera.h"
#include "game.h"


glm::vec3 direction;

unsigned int indices[] = {
};

void setup() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }

    Game::getInstance().setVertices({
            // Bottom hexagon
            0.0f, -0.5f,  0.0f,  1.0f, 0.0f, 0.0f,  // Red
            0.5f, -0.5f,  0.0f,  0.0f, 1.0f, 0.0f,  // Green
            0.25f, -0.5f,  0.433f,  0.0f, 0.0f, 1.0f,  // Blue
            -0.25f, -0.5f,  0.433f,  1.0f, 1.0f, 0.0f,  // Yellow
            -0.5f, -0.5f,  0.0f,  1.0f, 0.0f, 1.0f,  // Magenta
            -0.25f, -0.5f, -0.433f,  0.0f, 1.0f, 1.0f,  // Cyan
            0.25f, -0.5f, -0.433f,  0.5f, 0.5f, 0.5f,  // Gray

            // Top hexagon
            0.0f,  0.5f,  0.0f,  1.0f, 0.5f, 0.0f,  // Orange
            0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 0.5f,  // Teal
            0.25f,  0.5f,  0.433f,  0.5f, 0.0f, 1.0f,  // Purple
            -0.25f,  0.5f,  0.433f,  1.0f, 0.5f, 0.5f,  // Pink
            -0.5f,  0.5f,  0.0f,  0.5f, 1.0f, 0.5f,  // Light Green
            -0.25f,  0.5f, -0.433f,  0.5f, 0.5f, 1.0f,  // Light Blue
            0.25f,  0.5f, -0.433f,  1.0f, 1.0f, 1.0f   // White
    });



    Game::getInstance().setIndices({
       // Bottom hexagon
       0, 1, 2,
       0, 2, 3,
       0, 3, 4,
       0, 4, 5,
       0, 5, 6,
       0, 6, 1,

       // Top hexagon
       7, 8, 9,
       7, 9, 10,
       7, 10, 11,
       7, 11, 12,
       7, 12, 13,
       7, 13, 8,

       // Side faces
       1, 8, 9,
       1, 9, 2,
       2, 9, 10,
       2, 10, 3,
       3, 10, 11,
       3, 11, 4,
       4, 11, 12,
       4, 12, 5,
       5, 12, 13,
       5, 13, 6,
       6, 13, 8,
       6, 8, 1
    });



    Camera::getInstance().setPosition(glm::vec3(0.0f, 0.0f,  3.0f));
    Camera::getInstance().setFront(glm::vec3(0.0f, 0.0f, -1.0f));
    Camera::getInstance().setUp(glm::vec3(0.0f, 1.0f,  0.0f));
    Camera::getInstance().setSpeed(2.5);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}


int main() {
    setup();
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Open GL Learning Material", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos) {
        Game::getInstance().mouse_callback(window, xpos, ypos);
    });
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    std::filesystem::path contentRoot = std::filesystem::current_path().parent_path();
    std::filesystem::path vertexShaderPath = contentRoot / "shaders/shader.vert";
    std::filesystem::path fragmentShaderPath = contentRoot / "shaders/shader.frag";

    std::cout << "Vertex Shader Path: " << vertexShaderPath << std::endl;
    std::cout << "Fragment Shader Path: " << fragmentShaderPath << std::endl;

    if (!std::filesystem::exists(vertexShaderPath) || !std::filesystem::exists(fragmentShaderPath)) {
        std::cerr << "Shader files not found in the specified directory." << std::endl;
        return -1;
    }
    glEnable(GL_DEPTH_TEST);

    Shader shader(vertexShaderPath.string().c_str(), fragmentShaderPath.string().c_str());
    Game::getInstance().shader = &shader;

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, Game::getInstance().vertices.size() * sizeof(float), Game::getInstance().vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Game::getInstance().indices.size() * sizeof(unsigned int), Game::getInstance().indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); // TexCoord
    glEnableVertexAttribArray(1);
    Game::getInstance().VAO = VAO;

    shader.use();

    Game::getInstance().run(window);

    glfwTerminate();
    return 0;
}