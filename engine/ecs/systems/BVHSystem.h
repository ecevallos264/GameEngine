#ifndef GAMEENGINE_ECS_BVH_SYSTEM_H
#define GAMEENGINE_ECS_BVH_SYSTEM_H

#include "../../runtime/ISystem.h"
#include "../Registry.h"
#include "../components/TransformComponent.h"
#include "../components/BoundsComponent.h"
#include "../components/RenderComponent.h"
#include "../../core/spatial/BVHTree.h"
#include "../../rendering/SceneController.h"
#include "CameraSystem.h"
#include <glad/glad.h>

namespace ECS {

class BVHSystem : public ISystem {
public:
    explicit BVHSystem(CameraSystem& cameraSystem)
        : cameraSystem(cameraSystem) {}

    void initialize(SystemContext& context) override {
        (void)context;
        rebuildBVH();
    }

    void update(SystemContext& context) override {
        (void)context;

        // Check if we need to rebuild (when objects have moved significantly)
        if (needsRebuild || rebuildTimer >= rebuildInterval) {
            rebuildBVH();
            rebuildTimer = 0.0f;
            needsRebuild = false;
        }
        rebuildTimer += context.deltaTime;

        // Perform frustum culling using BVH
        performFrustumCulling();
    }

    // Force a rebuild next frame
    void markDirty() {
        needsRebuild = true;
    }

    // Get visible object count
    int getVisibleCount() const { return visibleCount; }

    // Get total object count
    int getTotalCount() const { return totalCount; }

    // Get BVH stats
    int getNodeCount() const { return static_cast<int>(bvhTree.nodeCount()); }
    int getTreeDepth() const { return bvhTree.getMaxDepth(); }

    // Access the BVH tree for visualization
    const Spatial::BVHTree& getBVHTree() const { return bvhTree; }

    // Access object info for visualization
    const std::vector<Spatial::BVHObjectInfo>& getObjects() const { return objects; }

    // Enable/disable BVH culling (for comparison)
    void setEnabled(bool enabled) { bvhEnabled = enabled; }
    bool isEnabled() const { return bvhEnabled; }

    // Set rebuild interval
    void setRebuildInterval(float interval) { rebuildInterval = interval; }

private:
    CameraSystem& cameraSystem;
    Spatial::BVHTree bvhTree;
    std::vector<Spatial::BVHObjectInfo> objects;

    bool needsRebuild = true;
    bool bvhEnabled = true;
    float rebuildTimer = 0.0f;
    float rebuildInterval = 0.5f; // Rebuild every 0.5 seconds

    int visibleCount = 0;
    int totalCount = 0;

    void rebuildBVH() {
        objects.clear();

        // Get registry from current scene
        auto currentScene = SceneController::getInstance().getCurrentScene();
        if (!currentScene) {
            totalCount = 0;
            bvhTree.clear();
            return;
        }

        Registry& registry = currentScene->getRegistry();

        // Gather all renderable entities with bounds
        registry.view<TransformComponent, BoundsComponent, RenderComponent>().each(
            [this](Entity entity, TransformComponent& transform,
                   BoundsComponent& bounds, RenderComponent& render) {
                (void)render;

                // Update world bounds from local bounds and transform
                updateWorldBounds(transform, bounds);

                // Create AABB from bounds component
                AABB aabb(bounds.worldMin, bounds.worldMax);
                objects.emplace_back(entity, aabb);
            }
        );

        totalCount = static_cast<int>(objects.size());

        // Build the BVH
        if (!objects.empty()) {
            bvhTree.build(objects, Spatial::BVHBuildMethod::SAH);
        } else {
            bvhTree.clear();
        }
    }

    void updateWorldBounds(TransformComponent& transform, BoundsComponent& bounds) {
        // Simple transformation: just translate local bounds by position
        // For full support, we'd need to handle rotation/scale properly
        bounds.worldMin = bounds.localMin * transform.scale + transform.position;
        bounds.worldMax = bounds.localMax * transform.scale + transform.position;

        // Handle negative scale
        glm::vec3 newMin = glm::min(bounds.worldMin, bounds.worldMax);
        glm::vec3 newMax = glm::max(bounds.worldMin, bounds.worldMax);
        bounds.worldMin = newMin;
        bounds.worldMax = newMax;
    }

    void performFrustumCulling() {
        // Get registry from current scene
        auto currentScene = SceneController::getInstance().getCurrentScene();
        if (!currentScene) return;

        Registry& registry = currentScene->getRegistry();

        if (!bvhEnabled || objects.empty()) {
            // If BVH disabled, mark all visible
            registry.view<BoundsComponent>().each(
                [](Entity entity, BoundsComponent& bounds) {
                    (void)entity;
                    bounds.visible = true;
                }
            );
            visibleCount = totalCount;
            return;
        }

        // Get frustum from camera
        Frustum frustum;
        if (!cameraSystem.getActiveFrustum(frustum)) {
            return;
        }

        // Reset all to invisible
        registry.view<BoundsComponent>().each(
            [](Entity entity, BoundsComponent& bounds) {
                (void)entity;
                bounds.visible = false;
            }
        );

        // Query BVH for visible objects
        std::vector<int> visibleIndices;
        bvhTree.queryFrustum(frustum, visibleIndices);

        // Mark visible objects
        visibleCount = 0;
        for (int idx : visibleIndices) {
            if (idx >= 0 && idx < static_cast<int>(objects.size())) {
                Entity entity = objects[idx].entity;
                auto* bounds = registry.get<BoundsComponent>(entity);
                if (bounds) {
                    bounds->visible = true;
                    visibleCount++;
                }
            }
        }
    }
};

// Debug renderer for BVH visualization
class BVHDebugRenderer {
public:
    void initialize() {
        // Create shader for debug lines
        const char* vertexShaderSrc = R"(
            #version 330 core
            layout (location = 0) in vec3 aPos;
            layout (location = 1) in vec4 aColor;
            out vec4 vertexColor;
            uniform mat4 view;
            uniform mat4 projection;
            void main() {
                gl_Position = projection * view * vec4(aPos, 1.0);
                vertexColor = aColor;
            }
        )";

        const char* fragmentShaderSrc = R"(
            #version 330 core
            in vec4 vertexColor;
            out vec4 FragColor;
            void main() {
                FragColor = vertexColor;
            }
        )";

        // Compile shaders
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSrc, nullptr);
        glCompileShader(vertexShader);

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSrc, nullptr);
        glCompileShader(fragmentShader);

        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        // Create VAO/VBO
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
    }

    void shutdown() {
        if (vao) glDeleteVertexArrays(1, &vao);
        if (vbo) glDeleteBuffers(1, &vbo);
        if (shaderProgram) glDeleteProgram(shaderProgram);
    }

    void render(const Spatial::BVHTree& bvh, const glm::mat4& view, const glm::mat4& projection,
                int maxDepth = -1, bool showLeafOnly = false) {
        if (bvh.empty()) return;

        vertices.clear();

        // Traverse BVH and collect box vertices
        bvh.traverse([this, maxDepth, showLeafOnly](const Spatial::BVHNode& node, int depth) {
            if (maxDepth >= 0 && depth > maxDepth) return;
            if (showLeafOnly && !node.isLeaf()) return;

            // Color based on depth
            glm::vec4 color = getDepthColor(depth);
            if (node.isLeaf()) {
                color = glm::vec4(0.0f, 1.0f, 0.0f, 0.8f); // Green for leaves
            }

            addBoxWireframe(node.bounds, color);
        });

        if (vertices.empty()) return;

        // Upload and render
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));

        glUseProgram(shaderProgram);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glLineWidth(1.0f);

        glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(vertices.size() / 7));

        glDisable(GL_BLEND);
        glBindVertexArray(0);
    }

private:
    GLuint shaderProgram = 0;
    GLuint vao = 0;
    GLuint vbo = 0;
    std::vector<float> vertices;

    glm::vec4 getDepthColor(int depth) {
        // Rainbow colors based on depth
        const glm::vec4 colors[] = {
            glm::vec4(1.0f, 0.0f, 0.0f, 0.6f),  // Red
            glm::vec4(1.0f, 0.5f, 0.0f, 0.6f),  // Orange
            glm::vec4(1.0f, 1.0f, 0.0f, 0.6f),  // Yellow
            glm::vec4(0.0f, 1.0f, 0.0f, 0.6f),  // Green
            glm::vec4(0.0f, 1.0f, 1.0f, 0.6f),  // Cyan
            glm::vec4(0.0f, 0.0f, 1.0f, 0.6f),  // Blue
            glm::vec4(0.5f, 0.0f, 1.0f, 0.6f),  // Purple
            glm::vec4(1.0f, 0.0f, 1.0f, 0.6f),  // Magenta
        };
        return colors[depth % 8];
    }

    void addBoxWireframe(const AABB& box, const glm::vec4& color) {
        glm::vec3 corners[8] = {
            glm::vec3(box.min.x, box.min.y, box.min.z),
            glm::vec3(box.max.x, box.min.y, box.min.z),
            glm::vec3(box.max.x, box.max.y, box.min.z),
            glm::vec3(box.min.x, box.max.y, box.min.z),
            glm::vec3(box.min.x, box.min.y, box.max.z),
            glm::vec3(box.max.x, box.min.y, box.max.z),
            glm::vec3(box.max.x, box.max.y, box.max.z),
            glm::vec3(box.min.x, box.max.y, box.max.z)
        };

        // 12 edges of a box
        int edges[12][2] = {
            {0, 1}, {1, 2}, {2, 3}, {3, 0}, // Bottom face
            {4, 5}, {5, 6}, {6, 7}, {7, 4}, // Top face
            {0, 4}, {1, 5}, {2, 6}, {3, 7}  // Vertical edges
        };

        for (auto& edge : edges) {
            addVertex(corners[edge[0]], color);
            addVertex(corners[edge[1]], color);
        }
    }

    void addVertex(const glm::vec3& pos, const glm::vec4& color) {
        vertices.push_back(pos.x);
        vertices.push_back(pos.y);
        vertices.push_back(pos.z);
        vertices.push_back(color.r);
        vertices.push_back(color.g);
        vertices.push_back(color.b);
        vertices.push_back(color.a);
    }
};

} // namespace ECS

#endif //GAMEENGINE_ECS_BVH_SYSTEM_H
