#ifndef SOFTWAREENGINEERINGPROJECT_VECTOR_LINE_H
#define SOFTWAREENGINEERINGPROJECT_VECTOR_LINE_H

#include "../shape.h"
#include "../shader-compiler.h"
#include "line.h"

class Vector : public Shape {
    float rotation;
    Line* line;
public:
    Vector(glm::vec3 position, float rotation, glm::vec3 color, float opacity) {
        this->position = position;
        this->rotation = rotation;
        float maxRenderDistance = 100.0f;
        this->line = new Line(glm::vec3(0.0f, maxRenderDistance, 0.0f) + position, glm::vec3(0.0f, -maxRenderDistance, 0.0f)  + position, color, opacity);

        this->line->initializeBuffers();
    }

    virtual void update(float deltaTime, Shader* shader) {
        this->line->update(deltaTime, shader);
    }

    void draw() {
        this->line->draw();
    }

    void setOpacity(float opacity) {
        this->line->setOpacity(opacity);
    }
};

#endif //SOFTWAREENGINEERINGPROJECT_VECTOR_LINE_H
