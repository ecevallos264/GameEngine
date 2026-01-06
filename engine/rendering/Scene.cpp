//
// Created by eceva on 1/14/2025.
//
#include "Scene.h"
#include "../ecs/components/TransformComponent.h"
#include "../ecs/components/MeshComponent.h"
#include "../ecs/components/RenderComponent.h"
#include "../camera/CameraHandler.h"
#include "../core/settings/settings.h"
#include <glad/glad.h>

int Scene::update(float deltaTime) {
    (void)deltaTime;
    // ECS entities are updated through systems
    return 0;
}

void Scene::render(glm::mat4 view, glm::mat4 projection) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Render all ECS entities with Transform, Mesh, and Render components
    registry.view<ECS::TransformComponent, ECS::MeshComponent, ECS::RenderComponent>().each(
        [&](ECS::Entity entity, ECS::TransformComponent& transform, ECS::MeshComponent& mesh, ECS::RenderComponent& render) {
            (void)entity;
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