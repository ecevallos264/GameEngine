#include "../engine/engine.h"
#include "entities/Player.h"
#include "../engine/core/eventing/EventDispatcher.h"
#include "scenes/TestScene.h"
#include "../engine/rendering/SceneController.h"

int main() {
    Engine::initGLFW();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(Settings::WINDOW_WIDTH, Settings::WINDOW_HEIGHT, Settings::WINDOW_TITLE, NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

//    EntityController::getInstance().addEntity(player);

    glfwMakeContextCurrent(window);
    Engine::initGLAD();

    ShaderInfo shaderInfo;
    shaderInfo.VertexShaderPath = "C:\\Users\\eceva\\CLionProjects\\GameEngine\\test_game\\shaders\\shader.vert";
    shaderInfo.FragmentShaderPath = "C:\\Users\\eceva\\CLionProjects\\GameEngine\\test_game\\shaders\\shader.frag";
//    shaderInfo.VertexShaderPath = "C:\\Users\\eceva\\CLionProjects\\3DGameEngineSolo1\\test_game\\shaders\\shader.vert";
//    shaderInfo.FragmentShaderPath = "C:\\Users\\eceva\\CLionProjects\\3DGameEngineSolo1\\test_game\\shaders\\shader.frag";
    std::shared_ptr<TestScene> scene = std::make_shared<TestScene>(
            new Shader(
                    shaderInfo.VertexShaderPath.c_str(),
                    shaderInfo.FragmentShaderPath.c_str()));
    SceneController::getInstance().addScene("testScene", scene);
    SceneController::getInstance().swapScene("testScene");

    EngineBuilder builder;

    InputHandler::setKeyPressCallback(window, [](GLFWwindow* window, int key, int scan, int action, int mods) {
//       std::cout << "Key {" << key << "} was pressed" << std::endl;
    });

    InputHandler::setMouseMovementCallback(window, [](GLFWwindow* window, int x, int y) {

    });
    glfwSwapInterval(0);
    Engine engine = builder
            .setShaderInfo(shaderInfo)
            .setCamera(new Camera(
                    glm::vec3(0.0f, 0.0f,  3.0f),
                    glm::vec3(0.0f, 0.0f, -1.0f),
                    glm::vec3(0.0f, 1.0f,  0.0f)))
            .setCameraSpeed(5.0f)
            .build(window);
    engine.run();

    return 0;
}

