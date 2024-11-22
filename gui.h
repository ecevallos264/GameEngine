 //
// Created by eceva on 11/21/2024.
//

#ifndef SOFTWAREENGINEERINGPROJECT_GUI_H
#define SOFTWAREENGINEERINGPROJECT_GUI_H

#include <glfw/glfw3.h>
#include "externals/imgui/imgui.h"
#include "settings.h"
#include "externals/imgui/imgui_impl_opengl3.h"

class GUI {
public:
    void render(GLFWwindow* window) {
        ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_FirstUseEver);
        ImGui::Begin("Project Controls");

        static float x = 0.0f, y = 0.0f, z = 0.0f;
        static float opacity = 1.0f;
        static float color[3] = {1.0f, 1.0f, 0.0f};
        static float rotation[3] = {0.0f, 0.0f, 0.0f};

        ImGui::InputFloat("X", &x);
        ImGui::InputFloat("Y", &y);
        ImGui::InputFloat("Z", &z);


        ImGui::SliderFloat("Opacity", &opacity, 0.0f, 1.0f);
        ImGui::ColorEdit3("Color", color);
        ImGui::SliderFloat3("Rotation", rotation, 0.0f, 360.0f);

        if (ImGui::Button("Go Back to Scene")) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            Settings::getInstance().CURSOR_FOCUS_STATUS = false;
        }

        ImGui::End();
        ImGui::Render();
//        vector->setPosition(glm::vec3(x, y, z));
//        vector->setRotation(glm::vec3(rotation[0], rotation[1], rotation[2]));
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
};

#endif //SOFTWAREENGINEERINGPROJECT_GUI_H
