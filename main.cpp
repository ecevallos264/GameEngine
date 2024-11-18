#include <iostream>
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <filesystem>
#include "shader-compiler.h"

#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH 800

float vertices[] = {
        // positions                                             // colors                                              // texture coords
        -0.5f, -0.5f, 0.0f,            1.0f, 0.0f, 0.0f,             0.0f, 0.0f,
        0.5f, -0.5f, 0.0f,          0.0f, 1.0f, 0.0f,       1.0f, 0.0f,
        0.5f,  0.5f, 0.0f,       0.0f, 0.0f, 1.0f,       1.0f, 1.0f,
        0.5f,  0.5f, 0.0f,       0.0f, 0.0f, 1.0f,       1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f,     1.0f, 1.0f, 0.0f,       0.0f, 1.0f,
        -0.5f, -0.5f, 0.0f,    1.0f, 0.0f, 0.0f,       0.0f, 0.0f
};

void renderGame(GLFWwindow* window, Shader &shader, unsigned int VAO, unsigned int VBO) {
    shader.use();
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
//    glBindTexture(GL_TEXTURE, texture);
    glBindVertexArray(0);
}

void setup() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }
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

    Shader shader(vertexShaderPath.string().c_str(), fragmentShaderPath.string().c_str());

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); // Color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); // TexCoord
    glEnableVertexAttribArray(2);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        renderGame(window, shader, VAO, VBO);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}