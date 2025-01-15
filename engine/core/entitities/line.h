#ifndef GAME_ENGINE_LINE_H
#define GAME_ENGINE_LINE_H

#include "shape.h"
#include "glad/glad.h"
#include "../shaders/shader-compiler.h"

class Line : public Shape {
    glm::vec3 rotation = glm::vec3(0.0f);
public:
    Line(const glm::vec3& start, const glm::vec3& end, glm::vec3 color, float opacity, Shader* shader) : Shape(shader) {
        vertices = {
                start.x, start.y, start.z, color.x, color.y, color.z, opacity,
                end.x, end.y, end.z, color.x, color.y, color.z, opacity,
        };
        indices = { 0, 1 };
        initializeBuffers();
    }

    void render(glm::mat4 view, glm::mat4 projection) override {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        this->shader->use();
        this->shader->setMat4("view", view);
        this->shader->setMat4("projection", projection);
        this->shader->setVec3("shapeColor", glm::vec3(1.0f, 0.0f, 0.0f));

        glBindVertexArray(this->getVAO());
        glDrawElements(GL_LINES, this->getIndices().size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void update(float deltaTime) override {
        glm::mat4 model = glm::mat4(1.0f);
        this->shader->setMat4("model", model);
    }

    void setOpacity(float opacity) {
        this->vertices[6] = opacity;
        this->vertices[13] = opacity;
    }

};

#endif //SOFTWAREENGINEERINGPROJECT_LINE_H
