#ifndef GAME_ENGINE_LINE_H
#define GAME_ENGINE_LINE_H

#include "shape.h"
#include "glad/glad.h"

class Line : public Shape {
    glm::vec3 rotation = glm::vec3(0.0f);
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

    void setOpacity(float opacity) {
        this->vertices[6] = opacity;
        this->vertices[13] = opacity;
    }

};

#endif //SOFTWAREENGINEERINGPROJECT_LINE_H
