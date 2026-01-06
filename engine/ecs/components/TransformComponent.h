#ifndef GAMEENGINE_ECS_TRANSFORM_COMPONENT_H
#define GAMEENGINE_ECS_TRANSFORM_COMPONENT_H

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace ECS {

struct TransformComponent {
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 rotation = glm::vec3(0.0f);  // Euler angles in degrees
    glm::vec3 scale = glm::vec3(1.0f);

    bool dirty = true;  // Flag to indicate transform needs recalculation

    TransformComponent() = default;

    TransformComponent(const glm::vec3& pos)
        : position(pos), dirty(true) {}

    TransformComponent(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scl)
        : position(pos), rotation(rot), scale(scl), dirty(true) {}

    glm::mat4 getModelMatrix() const {
        glm::mat4 model = glm::mat4(1.0f);

        // Translation
        model = glm::translate(model, position);

        // Rotation (ZYX order)
        model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));

        // Scale
        model = glm::scale(model, scale);

        return model;
    }

    void translate(const glm::vec3& delta) {
        position += delta;
        dirty = true;
    }

    void rotate(const glm::vec3& delta) {
        rotation += delta;
        dirty = true;
    }

    void setScale(const glm::vec3& newScale) {
        scale = newScale;
        dirty = true;
    }
};

} // namespace ECS

#endif //GAMEENGINE_ECS_TRANSFORM_COMPONENT_H
