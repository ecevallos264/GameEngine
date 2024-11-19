//
// Created by eceva on 11/19/2024.
//

#ifndef SOFTWAREENGINEERINGPROJECT_LINE_H
#define SOFTWAREENGINEERINGPROJECT_LINE_H

#include "../shape.h"

class Line : public Shape {
public:
    Line(const glm::vec3& start, const glm::vec3& end, glm::vec3 color, float opacity) {
        vertices = {
                start.x, start.y, start.z, color.x, color.y, color.z, opacity,
                end.x, end.y, end.z, color.x, color.y, color.z, opacity,
        };
        indices = { 0, 1 };
        initializeBuffers();
    }

    void draw() override {
        glBindVertexArray(this->getVAO());
        glDrawElements(GL_LINES, this->getIndices().size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void update(float deltaTime, Shader* shader) override {
        glm::mat4 model = glm::mat4(1.0f);
        shader->setMat4("model", model);
    }
};

#endif //SOFTWAREENGINEERINGPROJECT_LINE_H
