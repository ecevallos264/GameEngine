// Created by eceva on 1/6/2025.

#ifndef GAMEENGINE_ENGINE_H
#define GAMEENGINE_ENGINE_H

#include <glfw/glfw3.h>
#include <iostream>
#include <filesystem>
#include "camera/CameraHandler.h"
#include "core/shaders/ShaderInfo.h"
#include "glad/glad.h"
#include "core/settings/settings.h"
#include "core/shaders/shader-compiler.h"
#include "core/patterns/Singleton.h"
#include "game.h"
#include "core/eventing/EventDispatcher.h"
#include "core/eventing/events/KeyEvent.h"
#include "core/state/game_state.h"
#include "input/InputHandler.h"

class Engine {
public:
    static void initGLFW() {
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    static void initGLAD() {
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cerr << "Failed to initialize GLAD" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    void run() {
        Shader shader(shaderInfo.VertexShaderPath.c_str(), shaderInfo.FragmentShaderPath.c_str());
        Game::getInstance().shader = &shader;
        Game::getInstance().run(this->window);
        glfwTerminate();
    }

private:
    GLFWwindow* window;
    ShaderInfo shaderInfo;

    void validateShaders() {
        if (!std::filesystem::exists(shaderInfo.VertexShaderPath) || !std::filesystem::exists(shaderInfo.FragmentShaderPath)) {
            std::cerr << "Shader files not found in the specified directory." << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    void setupDepthTest() {
        glEnable(GL_DEPTH_TEST);
    }

    void setupViewport() {
        glViewport(0, 0, Settings::WINDOW_WIDTH, Settings::WINDOW_HEIGHT);
    }

public:
    Engine(ShaderInfo shaderInfo, GLFWwindow* window): shaderInfo(shaderInfo) {
        this->window = window;
        initGLFW();
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        initGLAD();
        validateShaders();
        setupDepthTest();
        setupViewport();
    }
};

class EngineBuilder {
private:
    ShaderInfo shaderInfo;
    Camera* camera;
    float cameraSpeed = 2.5f;

public:
    EngineBuilder& setShaderInfo(const ShaderInfo& shaderInfo) {
        this->shaderInfo = shaderInfo;
        return *this;
    }

    EngineBuilder& setCamera(Camera* camera) {
        this->camera = camera;
        return *this;
    }

    EngineBuilder& setCameraSpeed(float speed) {
        this->cameraSpeed = speed;
        return *this;
    }

    Engine build(GLFWwindow* window) {

        CameraHandler::getInstance().setCamera(camera);
        CameraHandler::getInstance().getCamera()->setSpeed(this->cameraSpeed);

        return Engine(shaderInfo, window);
    }
};



#endif //GAMEENGINE_ENGINE_H