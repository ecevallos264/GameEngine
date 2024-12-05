#ifndef SOFTWAREENGINEERINGPROJECT_GUI_H
#define SOFTWAREENGINEERINGPROJECT_GUI_H

#include <glfw/glfw3.h>
#include "externals/imgui/imgui.h"
#include "settings.h"
#include "externals/imgui/imgui_impl_opengl3.h"

struct Entity {
    bool isVisible;
    Shape* shape;
    std::string name;

    float x, y, z;
    float rotation[3];
    float opacity;
    float color[3];
};

class GUI {
private:
    std::vector<Entity> entities;
public:
void render(GLFWwindow* window) {
    ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_FirstUseEver);
    ImGui::Begin("Project Controls");

    static float x = 0.0f, y = 0.0f, z = 0.0f;
    static int selectedEntityType = 0;
    const char* entityTypes[] = {
            "Vector",
            "Hexagonal Prism",
            "Triangular Prism",
            "Pyramid",
            "Sphere"
    };

    float maxRenderDistance = Settings::getInstance().MAX_RENDER_DISTANCE;

    ImGui::SliderFloat("Overall Sensitivity", &Settings::getInstance().CURSOR_SENSITIVITY, 0, 2);
    ImGui::Combo("Entity Type", &selectedEntityType, entityTypes, IM_ARRAYSIZE(entityTypes));
    ImGui::SliderFloat("X##new", &x, -maxRenderDistance, maxRenderDistance);
    ImGui::SliderFloat("Y##new", &y, -maxRenderDistance, maxRenderDistance);
    ImGui::SliderFloat("Z##new", &z, -maxRenderDistance, maxRenderDistance);

    if (ImGui::Button("Add Entity")) {
        Shape* shape = nullptr;

        switch (selectedEntityType) {
            case 0:
                shape = new Vector(glm::vec3(x, y, z), glm::vec3(1.0f, 1.0f, 1.0f));
                break;
            case 1:
                shape = new HexagonalPrism(glm::vec3(x, y, z));
                break;
            case 2:
                shape = new TriangularPrism(glm::vec3(x, y, z));
                break;
            case 3:
                shape = new Pyramid(glm::vec3(x, y, z));
                break;
            case 4:
                shape = new Sphere(glm::vec3(x, y, z), 1.0f, 100, 0.5f);
                break;
        }

        EntityHandler::getInstance().addEntity(shape);
        this->entities.push_back({
                         true,
                         shape,
                         entityTypes[selectedEntityType],
                         x, y, z,
                         0.0f, 0.0f, 0.0f,
                         1.0f,
                 });
    }

    for (int i = 0; i < entities.size(); i++) {
        if(ImGui::CollapsingHeader((entities[i].name + '[' + std::to_string(i) + ']' + "##" + std::to_string(i)).c_str())) {
            Entity& entity = entities[i];
            ImGui::Text(entity.name.c_str());
            ImGui::SliderFloat(("X##" + std::to_string(i)).c_str(), &entity.x, -maxRenderDistance, maxRenderDistance);
            ImGui::SliderFloat(("Y##" + std::to_string(i)).c_str(), &entity.y, -maxRenderDistance, maxRenderDistance);
            ImGui::SliderFloat(("Z##" + std::to_string(i)).c_str(), &entity.z, -maxRenderDistance, maxRenderDistance);
            ImGui::SliderFloat(("Opacity##" + std::to_string(i)).c_str(), &entity.opacity, 0.0f, 1.0f);
            ImGui::ColorEdit3(("Color##" + std::to_string(i)).c_str(), entity.color);
            ImGui::SliderFloat3(("Rotation##" + std::to_string(i)).c_str(), entity.rotation, 0.0f, 360.0f);

            entity.shape->setPosition(glm::vec3(entity.x, entity.y, entity.z));
            entity.shape->setRotation(glm::vec3(entity.rotation[0], entity.rotation[1], entity.rotation[2]));
            entity.shape->setColor(glm::vec3(entity.color[0], entity.color[1], entity.color[2]));
            entity.shape->setOpacity(entity.opacity);

            if (ImGui::Button("Delete Entity")) {
                EntityHandler::getInstance().removeEntity(entity.shape);
                entities.erase(entities.begin() + i);
            }
        }

    }

    if (ImGui::Button("Go Back to Scene")) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        Settings::getInstance().CURSOR_FOCUS_STATUS = false;
    }

    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
};

#endif //SOFTWAREENGINEERINGPROJECT_GUI_H
