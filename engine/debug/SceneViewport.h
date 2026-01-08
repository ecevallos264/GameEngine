#ifndef GAMEENGINE_SCENE_VIEWPORT_H
#define GAMEENGINE_SCENE_VIEWPORT_H

#include "../rendering/Framebuffer.h"
#include "../rendering/SceneController.h"
#include "../ecs/systems/CameraSystem.h"
#include <imgui.h>
#include <glm/glm.hpp>

namespace Debug {

class SceneViewport {
public:
    SceneViewport() = default;

    bool initialize(int initialWidth, int initialHeight) {
        return framebuffer.initialize(initialWidth, initialHeight);
    }

    void shutdown() {
        framebuffer.destroy();
    }

    // Call before rendering the scene
    void beginSceneRender() {
        framebuffer.bind();
        glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    // Call after rendering the scene
    void endSceneRender() {
        framebuffer.unbind();
    }

    // Render the scene using the provided camera system
    void renderScene(ECS::CameraSystem* cameraSystem) {
        auto currentScene = SceneController::getInstance().getCurrentScene();
        if (!currentScene || !cameraSystem) return;

        if (!cameraSystem->getActiveCamera().isValid()) return;

        glm::mat4 view = cameraSystem->getViewMatrix();
        glm::mat4 projection = cameraSystem->getProjectionMatrix();

        currentScene->render(view, projection);
    }

    // Render just the viewport content (call from within an ImGui window)
    void renderContent(ECS::CameraSystem* cameraSystem) {
        // Get available size for the viewport
        ImVec2 availableSize = ImGui::GetContentRegionAvail();

        // Check if we need to resize the framebuffer
        int newWidth = static_cast<int>(availableSize.x);
        int newHeight = static_cast<int>(availableSize.y);

        if (newWidth > 0 && newHeight > 0) {
            if (newWidth != framebuffer.getWidth() || newHeight != framebuffer.getHeight()) {
                framebuffer.resize(newWidth, newHeight);

                // Update camera aspect ratio
                if (cameraSystem) {
                    cameraSystem->updateAspectRatio(
                        static_cast<float>(newWidth),
                        static_cast<float>(newHeight));
                }
            }

            // Display the framebuffer texture
            // Flip UV vertically because OpenGL textures are bottom-up
            ImGui::Image(
                (ImTextureID)(intptr_t)framebuffer.getColorTexture(),
                availableSize,
                ImVec2(0, 1),  // UV top-left (flipped)
                ImVec2(1, 0)   // UV bottom-right (flipped)
            );

            // Check if viewport is focused/hovered for input handling
            viewportFocused = ImGui::IsWindowFocused();
            viewportHovered = ImGui::IsWindowHovered();
        }
    }

    // Render the ImGui viewport window (standalone version)
    void renderImGuiWindow(ECS::CameraSystem* cameraSystem) {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

        ImGuiWindowFlags flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

        if (ImGui::Begin("Scene Viewport", nullptr, flags)) {
            renderContent(cameraSystem);
        }
        ImGui::End();

        ImGui::PopStyleVar();
    }

    void setClearColor(const glm::vec4& color) { clearColor = color; }

    bool isViewportFocused() const { return viewportFocused; }
    bool isViewportHovered() const { return viewportHovered; }

    int getWidth() const { return framebuffer.getWidth(); }
    int getHeight() const { return framebuffer.getHeight(); }

private:
    Framebuffer framebuffer;
    glm::vec4 clearColor{0.1f, 0.1f, 0.1f, 1.0f};
    bool viewportFocused = false;
    bool viewportHovered = false;
};

} // namespace Debug

#endif //GAMEENGINE_SCENE_VIEWPORT_H
