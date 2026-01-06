#ifndef GAMEENGINE_ECS_MESH_COMPONENT_H
#define GAMEENGINE_ECS_MESH_COMPONENT_H

#include <vector>
#include <glad/glad.h>
#include "../../entities/Vertex.h"

namespace ECS {

struct MeshComponent {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int EBO = 0;

    bool initialized = false;

    MeshComponent() = default;

    MeshComponent(const std::vector<Vertex>& verts, const std::vector<unsigned int>& inds)
        : vertices(verts), indices(inds) {}

    void initialize() {
        if (initialized) return;
        if (vertices.empty() || indices.empty()) return;

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        // Position attribute (location 0)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
        glEnableVertexAttribArray(0);

        // Color attribute (location 1)
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
        glEnableVertexAttribArray(1);

        // Alpha attribute (location 2)
        glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, alpha));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
        initialized = true;
    }

    void cleanup() {
        if (!initialized) return;
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteVertexArrays(1, &VAO);
        initialized = false;
        VAO = VBO = EBO = 0;
    }

    void updateVertexBuffer() {
        if (!initialized) return;
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    ~MeshComponent() {
        cleanup();
    }

    // Move semantics to handle OpenGL resources properly
    MeshComponent(MeshComponent&& other) noexcept
        : vertices(std::move(other.vertices))
        , indices(std::move(other.indices))
        , VAO(other.VAO)
        , VBO(other.VBO)
        , EBO(other.EBO)
        , initialized(other.initialized)
    {
        other.VAO = other.VBO = other.EBO = 0;
        other.initialized = false;
    }

    MeshComponent& operator=(MeshComponent&& other) noexcept {
        if (this != &other) {
            cleanup();
            vertices = std::move(other.vertices);
            indices = std::move(other.indices);
            VAO = other.VAO;
            VBO = other.VBO;
            EBO = other.EBO;
            initialized = other.initialized;
            other.VAO = other.VBO = other.EBO = 0;
            other.initialized = false;
        }
        return *this;
    }

    // Disable copy
    MeshComponent(const MeshComponent&) = delete;
    MeshComponent& operator=(const MeshComponent&) = delete;
};

} // namespace ECS

#endif //GAMEENGINE_ECS_MESH_COMPONENT_H
