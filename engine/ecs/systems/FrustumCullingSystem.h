#ifndef GAMEENGINE_ECS_FRUSTUM_CULLING_SYSTEM_H
#define GAMEENGINE_ECS_FRUSTUM_CULLING_SYSTEM_H

#include "../../runtime/ISystem.h"
#include "../Registry.h"
#include "../components/TransformComponent.h"
#include "../components/BoundsComponent.h"
#include "../components/RenderComponent.h"
#include "CameraSystem.h"
#include "../../core/math/Frustrum.h"

namespace ECS {

class FrustumCullingSystem : public ISystem {
public:
    FrustumCullingSystem(Registry& registry, CameraSystem& cameraSystem)
        : registry(registry), cameraSystem(cameraSystem) {}

    void update(SystemContext& context) override {
        (void)context;

        // Get active frustum from camera system
        Frustum frustum;
        if (!cameraSystem.getActiveFrustum(frustum)) {
            // No active camera, mark all as visible
            registry.view<BoundsComponent>().each(
                [](Entity entity, BoundsComponent& bounds) {
                    (void)entity;
                    bounds.visible = true;
                }
            );
            return;
        }

        // Update world bounds for entities with dirty transforms
        updateWorldBounds();

        // Perform frustum culling
        performCulling(frustum);
    }

    // Statistics for debugging
    struct CullingStats {
        size_t totalObjects = 0;
        size_t visibleObjects = 0;
        size_t culledObjects = 0;
    };

    CullingStats getStats() const {
        CullingStats stats;
        registry.view<BoundsComponent>().each(
            [&](Entity entity, BoundsComponent& bounds) {
                (void)entity;
                stats.totalObjects++;
                if (bounds.visible) {
                    stats.visibleObjects++;
                } else {
                    stats.culledObjects++;
                }
            }
        );
        return stats;
    }

    // Enable/disable frustum culling
    void setEnabled(bool enabled) { cullingEnabled = enabled; }
    bool isEnabled() const { return cullingEnabled; }

private:
    Registry& registry;
    CameraSystem& cameraSystem;
    bool cullingEnabled = true;

    void updateWorldBounds() {
        registry.view<TransformComponent, BoundsComponent>().each(
            [](Entity entity, TransformComponent& transform, BoundsComponent& bounds) {
                (void)entity;

                // Only update if transform or bounds are dirty
                if (!transform.dirty && !bounds.dirty) return;

                glm::mat4 model = transform.getModelMatrix();

                // Transform all 8 corners of the local AABB to world space
                glm::vec3 corners[8] = {
                    {bounds.localMin.x, bounds.localMin.y, bounds.localMin.z},
                    {bounds.localMax.x, bounds.localMin.y, bounds.localMin.z},
                    {bounds.localMin.x, bounds.localMax.y, bounds.localMin.z},
                    {bounds.localMax.x, bounds.localMax.y, bounds.localMin.z},
                    {bounds.localMin.x, bounds.localMin.y, bounds.localMax.z},
                    {bounds.localMax.x, bounds.localMin.y, bounds.localMax.z},
                    {bounds.localMin.x, bounds.localMax.y, bounds.localMax.z},
                    {bounds.localMax.x, bounds.localMax.y, bounds.localMax.z}
                };

                // Find world-space AABB by transforming corners and finding new min/max
                glm::vec4 worldCorner = model * glm::vec4(corners[0], 1.0f);
                bounds.worldMin = glm::vec3(worldCorner);
                bounds.worldMax = glm::vec3(worldCorner);

                for (int i = 1; i < 8; i++) {
                    worldCorner = model * glm::vec4(corners[i], 1.0f);
                    bounds.worldMin = glm::min(bounds.worldMin, glm::vec3(worldCorner));
                    bounds.worldMax = glm::max(bounds.worldMax, glm::vec3(worldCorner));
                }

                bounds.dirty = false;
            }
        );
    }

    void performCulling(const Frustum& frustum) {
        if (!cullingEnabled) {
            // If culling disabled, mark all as visible
            registry.view<BoundsComponent>().each(
                [](Entity entity, BoundsComponent& bounds) {
                    (void)entity;
                    bounds.visible = true;
                }
            );
            return;
        }

        registry.view<BoundsComponent>().each(
            [&](Entity entity, BoundsComponent& bounds) {
                (void)entity;
                bounds.visible = frustum.isAABBInside(bounds.worldMin, bounds.worldMax);
            }
        );
    }
};

} // namespace ECS

#endif //GAMEENGINE_ECS_FRUSTUM_CULLING_SYSTEM_H
