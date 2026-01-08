#ifndef GAMEENGINE_DEBUG_UI_H
#define GAMEENGINE_DEBUG_UI_H

#include <functional>
#include <string>
#include <vector>
#include <unordered_map>
#include "../core/patterns/Singleton.h"

struct GLFWwindow;

namespace Debug {

// Callback type for custom debug panels
using DebugPanelCallback = std::function<void()>;

class DebugUI : public Singleton<DebugUI> {
public:
    // Initialize ImGui with GLFW window
    void initialize(GLFWwindow* window);

    // Shutdown ImGui
    void shutdown();

    // Begin a new frame (call before any ImGui rendering)
    void beginFrame();

    // End frame and render (call after all ImGui windows)
    void endFrame();

    // Toggle the debug UI visibility
    void toggle() { visible = !visible; }

    // Check if debug UI is visible
    bool isVisible() const { return visible; }

    // Set visibility
    void setVisible(bool v) { visible = v; }

    // Register a custom debug panel
    // name: Display name in the menu
    // callback: Function that renders the panel content using ImGui
    void registerPanel(const std::string& name, DebugPanelCallback callback);

    // Unregister a debug panel
    void unregisterPanel(const std::string& name);

    // Built-in panels - can be enabled/disabled
    void setShowDemoWindow(bool show) { showDemoWindow = show; }
    void setShowMetricsWindow(bool show) { showMetricsWindow = show; }

private:
    bool initialized = false;
    bool visible = true;
    bool showDemoWindow = false;
    bool showMetricsWindow = false;
    bool showMainMenuBar = true;

    // Custom debug panels
    std::vector<std::pair<std::string, DebugPanelCallback>> panels;
    std::unordered_map<std::string, bool> panelVisibility;

    // Render the main menu bar
    void renderMainMenuBar();

    // Render all visible custom panels
    void renderPanels();
};

} // namespace Debug

#endif //GAMEENGINE_DEBUG_UI_H
