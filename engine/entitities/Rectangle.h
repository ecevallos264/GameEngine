#ifndef GAMEENGINE_RECTANGLE_H
#define GAMEENGINE_RECTANGLE_H

#include "Shape.h"

class Rectangle : public Shape {
protected:
    glm::vec3 min;
    glm::vec3 max;

public:
    Rectangle(Shader* shader, glm::vec3 pos, glm::vec3 size, glm::vec3 color)
            : Shape(shader), min(pos - size * 0.5f), max(pos + size * 0.5f) {

        glm::vec3 halfSize = size * 0.5f;
        vertices = {
                {{-halfSize.x, -halfSize.y,  halfSize.z}, color, 1.0f},
                {{ halfSize.x, -halfSize.y,  halfSize.z}, color, 1.0f},
                {{ halfSize.x,  halfSize.y,  halfSize.z}, color, 1.0f},
                {{-halfSize.x,  halfSize.y,  halfSize.z}, color, 1.0f},
                {{-halfSize.x, -halfSize.y, -halfSize.z}, color, 1.0f},
                {{ halfSize.x, -halfSize.y, -halfSize.z}, color, 1.0f},
                {{ halfSize.x,  halfSize.y, -halfSize.z}, color, 1.0f},
                {{-halfSize.x,  halfSize.y, -halfSize.z}, color, 1.0f}
        };

        indices = {
                0, 1, 2, 2, 3, 0,
                4, 5, 6, 6, 7, 4,
                0, 3, 7, 7, 4, 0,
                1, 5, 6, 6, 2, 1,
                3, 2, 6, 6, 7, 3,
                0, 1, 5, 5, 4, 0
        };

        updateVertexBuffer();
        initializeBuffers();
    }

    void render(glm::mat4 view, glm::mat4 projection) override {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, this->getPosition());

        this->shader->use();
        this->shader->setMat4("view", view);
        this->shader->setMat4("projection", projection);
        this->shader->setMat4("model", model);
        this->shader->setVec3("shapeColor", glm::vec3(1.0f, 0.0f, 0.0f));

        glBindVertexArray(this->getVAO());
        glDrawElements(GL_TRIANGLES, this->getIndices().size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void setOnUpdateCallback(std::function<void(double)> callback) {
        this->onUpdate = callback;
    }

    int update(float deltaTime) override {
        Shape::update(deltaTime);
        if(this->onUpdate) {
            this->onUpdate(deltaTime);
        }
    }

    glm::vec3 getSize() const {
        return max - min;
    }

    glm::vec3 getCenter() const {
        return (min + max) * 0.5f;
    }
};

#endif // GAMEENGINE_RECTANGLE_H
