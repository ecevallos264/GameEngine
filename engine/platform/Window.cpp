#include "Window.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>

Window::~Window() {
    shutdown();
}

bool Window::initialize(const WindowConfig& config) {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, config.glMajorVersion);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, config.glMinorVersion);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, config.resizable ? GLFW_TRUE : GLFW_FALSE);

    window = glfwCreateWindow(config.width, config.height, config.title.c_str(), nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    width = config.width;
    height = config.height;

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return false;
    }

    setVSync(config.vsync);

    glfwSetWindowUserPointer(window, this);

    glfwSetKeyCallback(window, glfwKeyCallback);
    glfwSetCursorPosCallback(window, glfwMouseMoveCallback);
    glfwSetMouseButtonCallback(window, glfwMouseButtonCallback);
    glfwSetScrollCallback(window, glfwScrollCallback);
    glfwSetFramebufferSizeCallback(window, glfwResizeCallback);

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, width, height);

    return true;
}

void Window::shutdown() {
    if (window) {
        glfwDestroyWindow(window);
        window = nullptr;
    }
    glfwTerminate();
}

bool Window::shouldClose() const {
    return window ? glfwWindowShouldClose(window) : true;
}

void Window::swapBuffers() {
    if (window) {
        glfwSwapBuffers(window);
    }
}

void Window::pollEvents() {
    glfwPollEvents();
}

int Window::getWidth() const {
    return width;
}

int Window::getHeight() const {
    return height;
}

float Window::getTime() const {
    return static_cast<float>(glfwGetTime());
}

void Window::setCursorMode(CursorMode mode) {
    if (!window) return;

    int glfwMode = GLFW_CURSOR_NORMAL;
    switch (mode) {
        case CursorMode::Normal:
            glfwMode = GLFW_CURSOR_NORMAL;
            break;
        case CursorMode::Hidden:
            glfwMode = GLFW_CURSOR_HIDDEN;
            break;
        case CursorMode::Disabled:
            glfwMode = GLFW_CURSOR_DISABLED;
            break;
    }
    glfwSetInputMode(window, GLFW_CURSOR, glfwMode);
}

void Window::setVSync(bool enabled) {
    glfwSwapInterval(enabled ? 1 : 0);
}

void Window::setTitle(const std::string& title) {
    if (window) {
        glfwSetWindowTitle(window, title.c_str());
    }
}

void* Window::getNativeHandle() const {
    return window;
}

void Window::setKeyCallback(KeyCallback callback) {
    keyCallback = std::move(callback);
}

void Window::setMouseMoveCallback(MouseMoveCallback callback) {
    mouseMoveCallback = std::move(callback);
}

void Window::setMouseButtonCallback(MouseButtonCallback callback) {
    mouseButtonCallback = std::move(callback);
}

void Window::setScrollCallback(ScrollCallback callback) {
    scrollCallback = std::move(callback);
}

void Window::setResizeCallback(ResizeCallback callback) {
    resizeCallback = std::move(callback);
}

void Window::glfwKeyCallback(GLFWwindow* glfwWindow, int key, int scancode, int action, int mods) {
    auto* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
    if (window && window->keyCallback) {
        window->keyCallback(key, scancode, action, mods);
    }
}

void Window::glfwMouseMoveCallback(GLFWwindow* glfwWindow, double x, double y) {
    auto* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
    if (window && window->mouseMoveCallback) {
        window->mouseMoveCallback(x, y);
    }
}

void Window::glfwMouseButtonCallback(GLFWwindow* glfwWindow, int button, int action, int mods) {
    auto* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
    if (window && window->mouseButtonCallback) {
        window->mouseButtonCallback(button, action, mods);
    }
}

void Window::glfwScrollCallback(GLFWwindow* glfwWindow, double xOffset, double yOffset) {
    auto* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
    if (window && window->scrollCallback) {
        window->scrollCallback(xOffset, yOffset);
    }
}

void Window::glfwResizeCallback(GLFWwindow* glfwWindow, int newWidth, int newHeight) {
    auto* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
    if (window) {
        window->width = newWidth;
        window->height = newHeight;
        glViewport(0, 0, newWidth, newHeight);
        if (window->resizeCallback) {
            window->resizeCallback(newWidth, newHeight);
        }
    }
}
