#ifndef SOFTWAREENGINEERINGPROJECT_PARTICLE_H
#define SOFTWAREENGINEERINGPROJECT_PARTICLE_H

#include "../shape.h"
#include "../shader-compiler.h"

class Particle : public Shape {
private:
    glm::vec3 direction;
public:
    Particle(glm::vec3 position, glm::vec3 direction) {
        this->position = position;
        this->direction = direction;
    }

    void update(float deltaTime, Shader* shader) override {
        // Call the parent class's update method
        Shape::update(deltaTime, shader);

        // Increment position based on direction and delta time
        position += direction * deltaTime;

        // Update the model matrix with the new position
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        shader->setMat4("model", model);
    }
};

#endif //SOFTWAREENGINEERINGPROJECT_PARTICLE_H