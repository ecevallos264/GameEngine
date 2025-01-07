// Created by eceva on 1/6/2025.

#ifndef GAMEENGINE_ENGINE_H
#define GAMEENGINE_ENGINE_H

#include <glfw/glfw3.h>
#include <iostream>
#include <filesystem>
#include "camera/camera.h"
#include "utils/shaders/ShaderInfo.h"
#include "glad/glad.h"
#include "utils/settings/settings.h"
#include "utils/shaders/shader-compiler.h"
#include "utils/patterns/Singleton.h"
#include "game/game.h"

class Engine {
private:
    ShaderInfo shaderInfo;

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

    void initCamera() {
        Camera::getInstance().setPosition(glm::vec3(0.0f, 0.0f,  3.0f));
        Camera::getInstance().setFront(glm::vec3(0.0f, 0.0f, -1.0f));
        Camera::getInstance().setUp(glm::vec3(0.0f, 1.0f,  0.0f));
        Camera::getInstance().setSpeed(2.5);
    }

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

    void runGame(GLFWwindow* window) {
        Shader shader(shaderInfo.VertexShaderPath.c_str(), shaderInfo.FragmentShaderPath.c_str());
        Game::getInstance().shader = &shader;
        Game::getInstance().run(window);
        glfwTerminate();
    }

    static void setMouseCallback(GLFWwindow* window, GLFWcursorposfun callback) {
        glfwSetCursorPosCallback(window, callback);
    }

    static void setOnKeyActionCallback(GLFWwindow* window, GLFWkeyfun callback) {
        glfwSetKeyCallback(window, callback);
    }

public:
    Engine(ShaderInfo shaderInfo, GLFWwindow* window): shaderInfo(shaderInfo) {
        initGLFW();
        initCamera();
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        initGLAD();
        validateShaders();
        setupDepthTest();
        setupViewport();
        runGame(window);
    }
};

class EngineBuilder {
private:
    ShaderInfo shaderInfo;
    glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    float cameraSpeed = 2.5f;

public:
    EngineBuilder& setShaderInfo(const ShaderInfo& shaderInfo) {
        this->shaderInfo = shaderInfo;
        return *this;
    }

    EngineBuilder& setCameraPosition(const glm::vec3& position) {
        this->cameraPosition = position;
        return *this;
    }

    EngineBuilder& setCameraFront(const glm::vec3& front) {
        this->cameraFront = front;
        return *this;
    }

    EngineBuilder& setCameraUp(const glm::vec3& up) {
        this->cameraUp = up;
        return *this;
    }

    EngineBuilder& setCameraSpeed(float speed) {
        this->cameraSpeed = speed;
        return *this;
    }

    EngineBuilder& setMouseCallback(GLFWwindow* window, GLFWcursorposfun callback) {
        glfwSetCursorPosCallback(window, callback);
        return *this;
    }

    EngineBuilder& setOnKeyActionCallback(GLFWwindow* window, GLFWkeyfun callback) {
        glfwSetKeyCallback(window, callback);
        return *this;
    }

    Engine build(GLFWwindow* window) {

        Camera::getInstance().setPosition(this->cameraPosition);
        Camera::getInstance().setFront(this->cameraFront);
        Camera::getInstance().setUp(this->cameraUp);
        Camera::getInstance().setSpeed(this->cameraSpeed);

        return Engine(shaderInfo, window);
    }
};



#endif //GAMEENGINE_ENGINE_H