#ifndef GAMEENGINE_ECS_MESH_RENDER_SYSTEM_H
#define GAMEENGINE_ECS_MESH_RENDER_SYSTEM_H

#include <glad/glad.h>
#include "../../runtime/ISystem.h"
#include "../Registry.h"
#include "../components/TransformComponent.h"
#include "../components/MeshComponent.h"
#include "../components/RenderComponent.h"
#include "../../camera/CameraHandler.h"
#include "../../core/settings/settings.h"

namespace ECS {

class MeshRenderSystem : public ISystem {
public:
    explicit MeshRenderSystem(Registry& registry) : registry(registry) {}

    void initialize(SystemContext& context) override {
        (void)context;
        // Initialize all mesh components that haven't been initialized
        auto* meshPool = registry.getPool<MeshComponent>();
        if (meshPool) {
            for (auto& mesh : meshPool->getComponents()) {
                mesh.initialize();
            }
        }
    }

    void render(SystemContext& context) override {
        (void)context;

        auto* camera = CameraHandler::getInstance().getCamera();
        if (!camera) return;

        glm::mat4 view = camera->getViewMatrix();
        glm::mat4 projection = camera->getProjectionMatrix();

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Iterate over all entities with Transform, Mesh, and Render components
        registry.view<TransformComponent, MeshComponent, RenderComponent>().each(
            [&](Entity entity, TransformComponent& transform, MeshComponent& mesh, RenderComponent& render) {
                if (!render.visible) return;
                if (!render.shader) return;

                // Initialize mesh if needed
                if (!mesh.initialized) {
                    mesh.initialize();
                }

                // Calculate model matrix
                glm::mat4 model = transform.getModelMatrix();

                // Setup shader
                render.shader->use();
                render.shader->setMat4("view", view);
                render.shader->setMat4("projection", projection);
                render.shader->setMat4("model", model);

                // Handle wireframe mode
                if (render.wireframe || Settings::DEBUG_RENDERING) {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                } else {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                }

                // Draw debug points if in debug mode
                if (Settings::DEBUG_RENDERING) {
                    glPointSize(10.0f);
                    glBindVertexArray(mesh.VAO);
                    glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(mesh.vertices.size()));
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                }

                // Draw mesh
                glBindVertexArray(mesh.VAO);
                glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh.indices.size()), GL_UNSIGNED_INT, nullptr);
                glBindVertexArray(0);
            }
        );

        glDisable(GL_BLEND);
    }

    void shutdown(SystemContext& context) override {
        (void)context;
        // Cleanup all mesh components
        auto* meshPool = registry.getPool<MeshComponent>();
        if (meshPool) {
            for (auto& mesh : meshPool->getComponents()) {
                mesh.cleanup();
            }
        }
    }

private:
    Registry& registry;
};

} // namespace ECS

#endif //GAMEENGINE_ECS_MESH_RENDER_SYSTEM_H
