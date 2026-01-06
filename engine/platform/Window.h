#ifndef GAMEENGINE_WINDOW_H
#define GAMEENGINE_WINDOW_H

#include <functional>
#include <string>
#include "WindowConfig.h"

struct GLFWwindow;

class Window {
public:
    Window() = default;
    ~Window();

    bool initialize(const WindowConfig& config);
    void shutdown();

    bool shouldClose() const;
    void swapBuffers();
    void pollEvents();

    int getWidth() const;
    int getHeight() const;
    float getTime() const;

    void setCursorMode(CursorMode mode);
    void setVSync(bool enabled);
    void setTitle(const std::string& title);

    void* getNativeHandle() const;

    using KeyCallback = std::function<void(int key, int scancode, int action, int mods)>;
    using MouseMoveCallback = std::function<void(double x, double y)>;
    using MouseButtonCallback = std::function<void(int button, int action, int mods)>;
    using ScrollCallback = std::function<void(double xOffset, double yOffset)>;
    using ResizeCallback = std::function<void(int width, int height)>;

    void setKeyCallback(KeyCallback callback);
    void setMouseMoveCallback(MouseMoveCallback callback);
    void setMouseButtonCallback(MouseButtonCallback callback);
    void setScrollCallback(ScrollCallback callback);
    void setResizeCallback(ResizeCallback callback);

private:
    GLFWwindow* window = nullptr;
    int width = 0;
    int height = 0;

    KeyCallback keyCallback;
    MouseMoveCallback mouseMoveCallback;
    MouseButtonCallback mouseButtonCallback;
    ScrollCallback scrollCallback;
    ResizeCallback resizeCallback;

    static void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void glfwMouseMoveCallback(GLFWwindow* window, double x, double y);
    static void glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void glfwScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
    static void glfwResizeCallback(GLFWwindow* window, int width, int height);
};

#endif //GAMEENGINE_WINDOW_H
