#include "../engine/engine.h"

int main() {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(Settings::WINDOW_WIDTH, Settings::WINDOW_HEIGHT, Settings::WINDOW_TITLE, NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);

    ShaderInfo shaderInfo;
    shaderInfo.VertexShaderPath = "C:\\Users\\eceva\\CLionProjects\\3DGameEngineSolo1\\test_game\\shaders\\shader.vert";
    shaderInfo.FragmentShaderPath = "C:\\Users\\eceva\\CLionProjects\\3DGameEngineSolo1\\test_game\\shaders\\shader.frag";

    EngineBuilder builder;
    Engine engine = builder
            .setShaderInfo(shaderInfo)
            .setCameraPosition(glm::vec3(5.0f, 5.0f, 5.0f))
            .setCameraFront(glm::vec3(0.0f, 0.0f, -1.0f))
            .setCameraSpeed(3.0f)
            .setMouseCallback(window)
            .build(window);

    return 0;
}

