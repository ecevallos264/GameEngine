#ifndef GAMEENGINE_FRUSTUM_H
#define GAMEENGINE_FRUSTUM_H

#include <cmath>
#include "Plane.h"
#include "glm/glm.hpp"
#include "../../rendering/partitioning/bvh-bounding-volume.h"
#include "../../camera/Camera.h"

class Frustum {
private:
    Plane topFace;
    Plane bottomFace;
    Plane leftFace;
    Plane rightFace;
    Plane nearFace;
    Plane farFace;

public:
    Plane getTopFace() const { return topFace; }
    Plane getBottomFace() const { return bottomFace; }
    Plane getLeftFace() const { return leftFace; }
    Plane getRightFace() const { return rightFace; }
    Plane getNearFace() const { return nearFace; }
    Plane getFarFace() const { return farFace; }

    static Frustum createFrustumFromCamera(const Camera& cam, float aspect, float fovY, float zNear, float zFar) {
        Frustum frustum;
        const float halfVSide = zFar * tanf(fovY * 0.5f);
        const float halfHSide = halfVSide * aspect;
        const glm::vec3 frontMultFar = zFar * cam.getFront();

        frustum.nearFace = Plane::fromPointNormal(cam.getPosition() + zNear * cam.getFront(), cam.getFront());
        frustum.farFace = Plane::fromPointNormal(cam.getPosition() + frontMultFar, -cam.getFront());
        frustum.rightFace = Plane::fromPointNormal(cam.getPosition(), glm::cross(frontMultFar - cam.getRight() * halfHSide, cam.getUp()));
        frustum.leftFace = Plane::fromPointNormal(cam.getPosition(), glm::cross(cam.getUp(), frontMultFar + cam.getRight() * halfHSide));
        frustum.topFace = Plane::fromPointNormal(cam.getPosition(), glm::cross(cam.getRight(), frontMultFar - cam.getUp() * halfVSide));
        frustum.bottomFace = Plane::fromPointNormal(cam.getPosition(), glm::cross(frontMultFar + cam.getUp() * halfVSide, cam.getRight()));

        return frustum;
    }

    bool isBoundingVolumeInside(const BVH::BoundingVolume* bounding_volume) const {
        if (!bounding_volume) return false;

        glm::vec3 min = bounding_volume->getMin();
        glm::vec3 max = bounding_volume->getMax();

        const Plane* planes[6] = { &topFace, &bottomFace, &leftFace, &rightFace, &nearFace, &farFace };

        for (const Plane* plane : planes) {
            glm::vec3 positiveVertex(
                    (plane->normal.x > 0) ? max.x : min.x,
                    (plane->normal.y > 0) ? max.y : min.y,
                    (plane->normal.z > 0) ? max.z : min.z
            );

            if (plane->distanceToPoint(positiveVertex) < 0) {
                return false;
            }
        }

        return true;
    }

    void drawFrusum(const glm::mat4& view, const glm::mat4& projection) {
        glm::mat4 invVP = glm::inverse(projection * view);

        glm::vec3 corners[8];
        int i = 0;
        for (int x = 0; x < 2; ++x) {
            for (int y = 0; y < 2; ++y) {
                for (int z = 0; z < 2; ++z) {
                    glm::vec4 corner = invVP * glm::vec4(
                            2.0f * x - 1.0f,
                            2.0f * y - 1.0f,
                            2.0f * z - 1.0f,
                            1.0f
                    );
                    corners[i++] = glm::vec3(corner) / corner.w;
                }
            }
        }

        // Define drawing color
        glm::vec4 edgeColor(1.0f, 0.4f, 0.0f, 1.0f);
        glm::vec4 quadColor(0.1f, 0.5f, 1.0f, 0.2f); // translucent blue

        std::vector<float> vertexData;

        auto push = [&](const glm::vec3& pos, const glm::vec4& color) {
            vertexData.push_back(pos.x);
            vertexData.push_back(pos.y);
            vertexData.push_back(pos.z);
            vertexData.push_back(color.r);
            vertexData.push_back(color.g);
            vertexData.push_back(color.b);
            vertexData.push_back(color.a);
        };

        // 12 edges as lines
        const int edges[12][2] = {
                {0, 1}, {1, 3}, {3, 2}, {2, 0}, // near
                {4, 5}, {5, 7}, {7, 6}, {6, 4}, // far
                {0, 4}, {1, 5}, {2, 6}, {3, 7}  // sides
        };
        for (auto& edge : edges) {
            push(corners[edge[0]], edgeColor);
            push(corners[edge[1]], edgeColor);
        }

        // Near plane quad (0 1 2 3)
        push(corners[0], quadColor); push(corners[1], quadColor); push(corners[2], quadColor);
        push(corners[2], quadColor); push(corners[3], quadColor); push(corners[0], quadColor);

        // Far plane quad (4 5 6 7)
        push(corners[4], quadColor); push(corners[5], quadColor); push(corners[6], quadColor);
        push(corners[6], quadColor); push(corners[7], quadColor); push(corners[4], quadColor);

        GLuint VAO, VBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0); // position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1); // color
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));

        Shader* shader = ShaderManager::getInstance().getShader("shader1");
        shader->use();
        shader->setMat4("model", glm::mat4(1.0f));
        shader->setMat4("view", view);
        shader->setMat4("projection", projection);

        glBindVertexArray(VAO);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glDrawArrays(GL_LINES, 0, 24);            // 12 edges
        glDrawArrays(GL_TRIANGLES, 24, 6);        // near plane
        glDrawArrays(GL_TRIANGLES, 30, 6);        // far plane

        glDisable(GL_BLEND);

        glBindVertexArray(0);
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

};

#endif
