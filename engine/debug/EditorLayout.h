#ifndef GAMEENGINE_EDITOR_LAYOUT_H
#define GAMEENGINE_EDITOR_LAYOUT_H

#include <imgui.h>
#include <functional>
#include <string>

namespace Debug {

struct LayoutConfig {
    float menuBarHeight = 19.0f;
    float leftPanelWidth = 250.0f;
    float rightPanelWidth = 300.0f;
    float bottomPanelHeight = 200.0f;
    bool showLeftPanel = true;
    bool showRightPanel = true;
    bool showBottomPanel = true;
};

class EditorLayout {
public:
    using PanelCallback = std::function<void()>;

    void beginFrame(int windowWidth, int windowHeight) {
        this->windowWidth = static_cast<float>(windowWidth);
        this->windowHeight = static_cast<float>(windowHeight);

        renderMenuBar();
    }

    void endFrame() {
        // Nothing needed here for now
    }

    // Render the left panel (e.g., Hierarchy)
    void renderLeftPanel(const char* title, PanelCallback content) {
        if (!config.showLeftPanel) return;

        ImGui::SetNextWindowPos(ImVec2(0, config.menuBarHeight));
        ImGui::SetNextWindowSize(ImVec2(config.leftPanelWidth, getContentHeight()));

        ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;

        if (ImGui::Begin(title, &config.showLeftPanel, flags)) {
            if (content) content();
        }
        ImGui::End();
    }

    // Render the right panel (e.g., Inspector)
    void renderRightPanel(const char* title, PanelCallback content) {
        if (!config.showRightPanel) return;

        float xPos = windowWidth - config.rightPanelWidth;
        ImGui::SetNextWindowPos(ImVec2(xPos, config.menuBarHeight));
        ImGui::SetNextWindowSize(ImVec2(config.rightPanelWidth, getContentHeight()));

        ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;

        if (ImGui::Begin(title, &config.showRightPanel, flags)) {
            if (content) content();
        }
        ImGui::End();
    }

    // Render the bottom panel (e.g., Console)
    void renderBottomPanel(const char* title, PanelCallback content) {
        if (!config.showBottomPanel) return;

        float xPos = config.showLeftPanel ? config.leftPanelWidth : 0;
        float width = getCenterWidth();
        float yPos = windowHeight - config.bottomPanelHeight;

        ImGui::SetNextWindowPos(ImVec2(xPos, yPos));
        ImGui::SetNextWindowSize(ImVec2(width, config.bottomPanelHeight));

        ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;

        if (ImGui::Begin(title, &config.showBottomPanel, flags)) {
            if (content) content();
        }
        ImGui::End();
    }

    // Get the center viewport area dimensions
    ImVec2 getCenterPosition() const {
        float x = config.showLeftPanel ? config.leftPanelWidth : 0;
        float y = config.menuBarHeight;
        return ImVec2(x, y);
    }

    ImVec2 getCenterSize() const {
        float width = getCenterWidth();
        float height = getCenterHeight();
        return ImVec2(width, height);
    }

    // Render center viewport window
    void renderCenterViewport(const char* title, PanelCallback content) {
        ImVec2 pos = getCenterPosition();
        ImVec2 size = getCenterSize();

        ImGui::SetNextWindowPos(pos);
        ImGui::SetNextWindowSize(size);

        ImGuiWindowFlags flags =
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoScrollbar |
            ImGuiWindowFlags_NoScrollWithMouse;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

        if (ImGui::Begin(title, nullptr, flags)) {
            if (content) content();
        }
        ImGui::End();

        ImGui::PopStyleVar();
    }

    LayoutConfig& getConfig() { return config; }

private:
    LayoutConfig config;
    float windowWidth = 1600.0f;
    float windowHeight = 1200.0f;

    float getContentHeight() const {
        return windowHeight - config.menuBarHeight;
    }

    float getCenterWidth() const {
        float width = windowWidth;
        if (config.showLeftPanel) width -= config.leftPanelWidth;
        if (config.showRightPanel) width -= config.rightPanelWidth;
        return width;
    }

    float getCenterHeight() const {
        float height = windowHeight - config.menuBarHeight;
        if (config.showBottomPanel) height -= config.bottomPanelHeight;
        return height;
    }

    void renderMenuBar() {
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("New Scene")) {}
                if (ImGui::MenuItem("Open Scene")) {}
                if (ImGui::MenuItem("Save Scene")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Exit")) {}
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Edit")) {
                if (ImGui::MenuItem("Undo", "Ctrl+Z")) {}
                if (ImGui::MenuItem("Redo", "Ctrl+Y")) {}
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("View")) {
                ImGui::MenuItem("Hierarchy", nullptr, &config.showLeftPanel);
                ImGui::MenuItem("Inspector", nullptr, &config.showRightPanel);
                ImGui::MenuItem("Console", nullptr, &config.showBottomPanel);
                ImGui::Separator();
                if (ImGui::MenuItem("Reset Layout")) {
                    config = LayoutConfig();
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Help")) {
                if (ImGui::MenuItem("About")) {}
                ImGui::EndMenu();
            }

            // Show FPS on the right side of menu bar
            ImGui::SameLine(windowWidth - 100);
            ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);

            ImGui::EndMainMenuBar();
        }
    }
};

} // namespace Debug

#endif //GAMEENGINE_EDITOR_LAYOUT_H
