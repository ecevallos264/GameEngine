#ifndef GAME_ENGINE_XPLANE_H
#define GAME_ENGINE_XPLANE_H

// entities/plane.h
#ifndef GAME_ENGINE_PLANE_H
#define GAME_ENGINE_PLANE_H

#include "../shape.h"
#include "glad/glad.h"
#include "../shader-compiler.h"

class xPlane : public Shape {
private:
    float opacity;
public:
    xPlane(const glm::vec3& position, float width, float height, const glm::vec3& color, int gridDivisions, float opacity) {
        this->opacity = opacity;
        float halfWidth = width / 2.0f;
        float halfHeight = height / 2.0f;
        float stepX = width / gridDivisions;
        float stepZ = height / gridDivisions;

        // Generate vertices
        for (int i = 0; i <= gridDivisions; ++i) {
            for (int j = 0; j <= gridDivisions; ++j) {
                float x = -halfWidth + i * stepX;
                float z = -halfHeight + j * stepZ;
                vertices.push_back(x);
                vertices.push_back(0.0f);
                vertices.push_back(z);
                vertices.push_back(color.x);
                vertices.push_back(color.y);
                vertices.push_back(color.z);
                vertices.push_back(opacity);
            }
        }

        // Generate indices
        for (int i = 0; i < gridDivisions; ++i) {
            for (int j = 0; j < gridDivisions; ++j) {
                int start = i * (gridDivisions + 1) + j;
                indices.push_back(start);
                indices.push_back(start + 1);
                indices.push_back(start + gridDivisions + 1);

                indices.push_back(start + 1);
                indices.push_back(start + gridDivisions + 2);
                indices.push_back(start + gridDivisions + 1);
            }
        }

        setPosition(position);
        initializeBuffers();
    }

    void draw() override {
        glBindVertexArray(this->getVAO());
        glDrawElements(GL_TRIANGLES, this->getIndices().size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void update(float deltaTime, Shader* shader) override {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, this->getPosition());
        shader->setMat4("model", model);
    }
};

#endif //SOFTWAREENGINEERINGPROJECT_PLANE_H

#endif //SOFTWAREENGINEERINGPROJECT_XPLANE_H
