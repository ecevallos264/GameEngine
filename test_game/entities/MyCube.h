//
// Created by eceva on 1/17/2025.
//

#ifndef GAMEENGINE_MYCUBE_H
#define GAMEENGINE_MYCUBE_H

#include "../../engine/entitities/shape.h"
#include "../../engine/physics/CollisionHandler.h"

struct MyCubeCollision {

};

class MyCube : public Shape {
private:
    void registerCollisionListener() {
        CollisionHandler::getInstance().subscribe<MyCube>(typeid(MyCube), [this](CollisionData data) {
            this->onCollision(static_cast<MyCube *>(data.entityB));
        });
    }

public:
    MyCube(glm::vec3 pos, Shader* shader, glm::vec3 color) : Shape(shader) {
        position = pos;
        this->vertices.push_back({glm::vec3(-0.5f, -0.5f,  0.5f), color, 1.0f});
        this->vertices.push_back({glm::vec3(0.5f, -0.5f,  0.5f), color, 1.0f});
        this->vertices.push_back({glm::vec3(0.5f, 0.5f,  0.5f), color, 1.0f});
        this->vertices.push_back({glm::vec3(-0.5f, 0.5f,  0.5f), color, 1.0f});

        this->vertices.push_back({glm::vec3(-0.5f, -0.5f,  -0.5f), color, 1.0f});
        this->vertices.push_back({glm::vec3(0.5f, -0.5f,  -0.5f), color, 1.0f});
        this->vertices.push_back({glm::vec3(0.5f, 0.5f,  -0.5f), color, 1.0f});
        this->vertices.push_back({glm::vec3(-0.5f, 0.5f,  -0.5f), color, 1.0f});
        updateVertexBuffer();

        indices = {
                0, 1, 2, 2, 3, 0,
                4, 5, 6, 6, 7, 4,
                0, 3, 7, 7, 4, 0,
                1, 5, 6, 6, 2, 1,
                3, 2, 6, 6, 7, 3,
                0, 1, 5, 5, 4, 0
        };
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



    void update(float deltaTime) override {
        Shape::update(deltaTime);
        if(this->onUpdate) {
            this->onUpdate(deltaTime);
        }
    }

    void onCollision(MyCube* entity) {
        std::cout << "Collision Detected" << std::endl;
    }

    static std::type_index getType() {
        return typeid(MyCube);
    }
};


#endif //GAMEENGINE_MYCUBE_H
