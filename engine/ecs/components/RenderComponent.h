#ifndef GAMEENGINE_ECS_RENDER_COMPONENT_H
#define GAMEENGINE_ECS_RENDER_COMPONENT_H

#include <glm/vec3.hpp>
#include <string>
#include "../../core/shaders/shader-compiler.h"

namespace ECS {

struct RenderComponent {
    Shader* shader = nullptr;
    glm::vec3 color = glm::vec3(1.0f);
    float opacity = 1.0f;
    bool visible = true;
    bool wireframe = false;

    RenderComponent() = default;

    RenderComponent(Shader* shdr)
        : shader(shdr) {}

    RenderComponent(Shader* shdr, const glm::vec3& col, float alpha = 1.0f)
        : shader(shdr), color(col), opacity(alpha) {}
};

} // namespace ECS

#endif //GAMEENGINE_ECS_RENDER_COMPONENT_H
