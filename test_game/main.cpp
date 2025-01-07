#include "../engine/engine.h"

//void Game::mouse_callback(GLFWwindow* window, double xpos, double ypos)
//{
//
//    bool cursorInWindow = (xpos >= 0 && xpos <= WINDOW_WIDTH && ypos >= 0 && ypos <= WINDOW_HEIGHT);
//
//    if (!Settings::getInstance().CURSOR_FOCUS_STATUS) {
//        if (firstMouse)
//        {
//            lastX = xpos;
//            lastY = ypos;
//            firstMouse = false;
//        }
//        this->xPos = xpos;
//        this->yPos = ypos;
//
//        float xoffset = xpos - lastX;
//        float yoffset = lastY - ypos;
//        lastX = xpos;
//        lastY = ypos;
//
//        float sensitivity = Settings::getInstance().CURSOR_SENSITIVITY;
//        xoffset *= sensitivity;
//        yoffset *= sensitivity;
//
//        Camera::getInstance().yaw   += xoffset;
//        Camera::getInstance().pitch += yoffset;
//
//        if(Camera::getInstance().pitch > 89.0f)
//            Camera::getInstance().pitch = 89.0f;
//        if(Camera::getInstance().pitch < -89.0f)
//            Camera::getInstance().pitch = -89.0f;
//
//        glm::vec3 direction;
//        direction.x = cos(glm::radians(Camera::getInstance().yaw)) * cos(glm::radians(Camera::getInstance().pitch));
//        direction.y = sin(glm::radians(Camera::getInstance().pitch));
//        direction.z = sin(glm::radians(Camera::getInstance().yaw)) * cos(glm::radians(Camera::getInstance().pitch));
//        Camera::getInstance().setFront(glm::normalize(direction));
//    } else {
//        firstMouse = true;
//    }
//}

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
            .setMouseOnEventCallback(window)
            .build(window);

    return 0;
}

