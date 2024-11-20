#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <filesystem>
#include "shader-compiler.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include "camera.h"
#include "game.h"
#include "externals/imgui/imgui.h"
#include "externals/imgui/backends/imgui_impl_glfw.h"
#include "externals/imgui/backends/imgui_impl_opengl3.h"

void setup() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }

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

    //ImGui Setup
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    Game::getInstance().run(window);
    glfwTerminate();
    return 0;
}