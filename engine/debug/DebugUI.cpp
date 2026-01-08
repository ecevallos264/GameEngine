#include "DebugUI.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace Debug {

void DebugUI::initialize(GLFWwindow* window) {
    if (initialized) return;

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable keyboard navigation

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Customize style for better visibility
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 5.0f;
    style.FrameRounding = 3.0f;
    style.ScrollbarRounding = 3.0f;

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    initialized = true;
}

void DebugUI::shutdown() {
    if (!initialized) return;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    initialized = false;
}

void DebugUI::beginFrame() {
    if (!initialized || !visible) return;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void DebugUI::endFrame() {
    if (!initialized || !visible) return;

    // Render main menu bar
    if (showMainMenuBar) {
        renderMainMenuBar();
    }

    // Render custom panels
    renderPanels();

    // Show demo window if enabled
    if (showDemoWindow) {
        ImGui::ShowDemoWindow(&showDemoWindow);
    }

    // Show metrics window if enabled
    if (showMetricsWindow) {
        ImGui::ShowMetricsWindow(&showMetricsWindow);
    }

    // Render ImGui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void DebugUI::registerPanel(const std::string& name, DebugPanelCallback callback) {
    // Check if panel already exists
    for (auto& panel : panels) {
        if (panel.first == name) {
            panel.second = callback;
            return;
        }
    }

    panels.emplace_back(name, callback);
    panelVisibility[name] = true;  // Visible by default
}

void DebugUI::unregisterPanel(const std::string& name) {
    panels.erase(
        std::remove_if(panels.begin(), panels.end(),
            [&name](const auto& p) { return p.first == name; }),
        panels.end()
    );
    panelVisibility.erase(name);
}

void DebugUI::renderMainMenuBar() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Debug")) {
            ImGui::MenuItem("Show Demo Window", nullptr, &showDemoWindow);
            ImGui::MenuItem("Show Metrics", nullptr, &showMetricsWindow);
            ImGui::Separator();

            // List all custom panels
            if (!panels.empty()) {
                for (auto& [name, callback] : panels) {
                    bool& isVisible = panelVisibility[name];
                    ImGui::MenuItem(name.c_str(), nullptr, &isVisible);
                }
                ImGui::Separator();
            }

            if (ImGui::MenuItem("Hide Debug UI", "F3")) {
                visible = false;
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Windows")) {
            for (auto& [name, callback] : panels) {
                bool& isVisible = panelVisibility[name];
                ImGui::MenuItem(name.c_str(), nullptr, &isVisible);
            }
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

void DebugUI::renderPanels() {
    for (auto& [name, callback] : panels) {
        if (panelVisibility[name]) {
            if (ImGui::Begin(name.c_str(), &panelVisibility[name])) {
                callback();
            }
            ImGui::End();
        }
    }
}

} // namespace Debug
