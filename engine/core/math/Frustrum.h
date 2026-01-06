#ifndef GAMEENGINE_FRUSTUM_H
#define GAMEENGINE_FRUSTUM_H

#include <cmath>
#include "Plane.h"
#include "glm/glm.hpp"
#include <glm/gtx/string_cast.hpp>
#include "../../camera/Camera.h"
#include <stdexcept>

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
        // Validate input parameters
        if (aspect <= 0.0f) {
            throw std::invalid_argument("Aspect ratio must be greater than 0.");
        }
        if (fovY <= 0.0f || fovY >= 180.0f) {
            throw std::invalid_argument("Field of view (fovY) must be in the range (0, 180) degrees.");
        }
        if (zNear <= 0.0f || zFar <= zNear) {
            throw std::invalid_argument("zNear must be greater than 0 and less than zFar.");
        }

        // Validate camera vectors
        if (glm::length(cam.getFront()) == 0.0f) {
            throw std::invalid_argument("Camera front vector must not be a zero vector.");
        }
        if (glm::length(cam.getUp()) == 0.0f) {
            throw std::invalid_argument("Camera up vector must not be a zero vector.");
        }
        if (glm::length(glm::cross(cam.getFront(), cam.getUp())) == 0.0f) {
            throw std::invalid_argument("Camera front and up vectors must not be collinear.");
        }

        Frustum frustum;
        const float halfVSide = zFar * tanf(glm::radians(fovY) * 0.5f);
        const float halfHSide = halfVSide * aspect;
        const glm::vec3 frontMultFar = zFar * cam.getFront();

//        // Near plane
//        frustum.nearFace = Plane::fromPointNormal(
//                cam.getPosition() + zNear * cam.getFront(),
//                cam.getFront()
//        );
//
//        // Far plane
//        frustum.farFace = Plane::fromPointNormal(
//                cam.getPosition() + frontMultFar,
//                -cam.getFront()
//        );
//
//        // Right plane
//        frustum.rightFace = Plane::fromPointNormal(
//                cam.getPosition(),
//                glm::cross(cam.getUp(), frontMultFar - cam.getRight() * halfHSide)
//        );
//
//        // Left plane
//        frustum.leftFace = Plane::fromPointNormal(
//                cam.getPosition(),
//                glm::cross(frontMultFar + cam.getRight() * halfHSide, cam.getUp())
//        );
//
//        // Top plane
//        frustum.topFace = Plane::fromPointNormal(
//                cam.getPosition(),
//                glm::cross(cam.getRight(), frontMultFar - cam.getUp() * halfVSide)
//        );
//
//        // Bottom plane
//        frustum.bottomFace = Plane::fromPointNormal(
//                cam.getPosition(),
//                glm::cross(frontMultFar + cam.getUp() * halfVSide, cam.getRight())
//        );

        glm::vec3 pos = cam.getPosition();
        glm::vec3 front = glm::normalize(cam.getFront());
        glm::vec3 right = glm::normalize(cam.getRight());
        glm::vec3 up    = glm::normalize(cam.getUp());

        glm::vec3 farCenter  = pos + front * zFar;
        glm::vec3 nearCenter = pos + front * zNear;

        // Near
        frustum.nearFace = Plane::fromPointNormal(
                nearCenter,
                front
        );

        // Far
        frustum.farFace = Plane::fromPointNormal(
                farCenter,
                -front
        );

        // Right
        frustum.rightFace = Plane::fromPointNormal(
                pos,
                glm::cross(up, farCenter + right * halfHSide - pos)
        );

        // Left
        frustum.leftFace = Plane::fromPointNormal(
                pos,
                glm::cross(farCenter - right * halfHSide - pos, up)
        );

        // Top
        frustum.topFace = Plane::fromPointNormal(
                pos,
                glm::cross(farCenter - up * halfVSide - pos, right)
        );

        // Bottom
        frustum.bottomFace = Plane::fromPointNormal(
                pos,
                glm::cross(right, farCenter + up * halfVSide - pos)
        );

        return frustum;
    }

    // Check if an AABB is inside the frustum
    bool isAABBInside(const glm::vec3& min, const glm::vec3& max) const {
        constexpr float EPSILON = 1e-3f;

        const Plane* planes[6] = { &topFace, &bottomFace, &leftFace, &rightFace, &nearFace, &farFace };

        for (int i = 0; i < 6; ++i) {
            const Plane* plane = planes[i];
            glm::vec3 normal = plane->normal;

            glm::vec3 positiveVertex(
                    (normal.x >= 0) ? max.x : min.x,
                    (normal.y >= 0) ? max.y : min.y,
                    (normal.z >= 0) ? max.z : min.z
            );

            float distance = plane->distanceToPoint(positiveVertex);

            if (distance < -EPSILON) {
                return false;
            }
        }

        return true;
    }


    void drawFrustrum(const glm::mat4& view, const glm::mat4& projection) {
        static bool showGhostFrustum = true;
        static bool gKeyPressed = false;

        // Handle key toggle (GLFW example)
        if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_G) == GLFW_PRESS && !gKeyPressed) {
            showGhostFrustum = !showGhostFrustum;
            gKeyPressed = true;
        }
        if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_G) == GLFW_RELEASE) {
            gKeyPressed = false;
        }

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

        // Compute center of frustum
        glm::vec3 center(0.0f);
        for (int j = 0; j < 8; ++j) center += corners[j];
        center /= 8.0f;

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

        glm::vec4 edgeColor(1.0f, 0.4f, 0.0f, 1.0f);
        glm::vec4 quadColor(0.1f, 0.5f, 1.0f, 0.2f); // translucent blue

        const int edges[12][2] = {
                {0, 1}, {1, 3}, {3, 2}, {2, 0}, // near
                {4, 5}, {5, 7}, {7, 6}, {6, 4}, // far
                {0, 4}, {1, 5}, {2, 6}, {3, 7}  // sides
        };

        // Push main frustum edges
        for (auto& edge : edges) {
            push(corners[edge[0]], edgeColor);
            push(corners[edge[1]], edgeColor);
        }

        // Push main frustum near/far planes
        push(corners[0], quadColor); push(corners[1], quadColor); push(corners[2], quadColor);
        push(corners[2], quadColor); push(corners[3], quadColor); push(corners[0], quadColor);

        push(corners[4], quadColor); push(corners[5], quadColor); push(corners[6], quadColor);
        push(corners[6], quadColor); push(corners[7], quadColor); push(corners[4], quadColor);

        // Optional: draw ghost frustum
        if (showGhostFrustum) {
            glm::vec3 ghostCorners[8];
            float ghostScale = 0.7f;

            glm::vec3 forward = glm::normalize(corners[1] - corners[0]);
            glm::vec3 offset = forward * 0.5f;

            for (int j = 0; j < 8; ++j) {
                ghostCorners[j] = center + ghostScale * (corners[j] - center) + offset;
            }

            glm::vec4 ghostEdgeColor(0.0f, 1.0f, 0.0f, 0.8f); // solid green
            glm::vec4 ghostQuadColor(0.0f, 1.0f, 0.0f, 0.2f); // translucent green

            for (auto& edge : edges) {
                push(ghostCorners[edge[0]], ghostEdgeColor);
                push(ghostCorners[edge[1]], ghostEdgeColor);
            }

            // Near plane
            push(ghostCorners[0], ghostQuadColor); push(ghostCorners[1], ghostQuadColor); push(ghostCorners[2], ghostQuadColor);
            push(ghostCorners[2], ghostQuadColor); push(ghostCorners[3], ghostQuadColor); push(ghostCorners[0], ghostQuadColor);

            // Far plane
            push(ghostCorners[4], ghostQuadColor); push(ghostCorners[5], ghostQuadColor); push(ghostCorners[6], ghostQuadColor);
            push(ghostCorners[6], ghostQuadColor); push(ghostCorners[7], ghostQuadColor); push(ghostCorners[4], ghostQuadColor);
        }

        // Upload and render
        GLuint VAO, VBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));

        Shader* shader = ShaderManager::getInstance().getShader("shader1");
        shader->use();
        shader->setMat4("model", glm::mat4(1.0f));
        shader->setMat4("view", view);
        shader->setMat4("projection", projection);

        glBindVertexArray(VAO);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glDrawArrays(GL_LINES, 0, 24);     // main edges
        glDrawArrays(GL_TRIANGLES, 24, 6); // main near
        glDrawArrays(GL_TRIANGLES, 30, 6); // main far

        if (showGhostFrustum) {
            glDrawArrays(GL_LINES, 36, 24);     // ghost edges
            glDrawArrays(GL_TRIANGLES, 60, 6);  // ghost near
            glDrawArrays(GL_TRIANGLES, 66, 6);  // ghost far
        }

        glDisable(GL_BLEND);
        glBindVertexArray(0);
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
};

#endif
