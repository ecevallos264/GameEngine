//
// Created by eceva on 1/17/2025.
//

#ifndef GAMEENGINE_MYCUBE_H
#define GAMEENGINE_MYCUBE_H

#include <GLFW/glfw3.h>
#include <GL/gl.h>

#include "../../engine/entitities/shape.h"
#include "../../engine/physics/CollisionHandler.h"
#include "../../engine/entitities/Point.h"


class MyCube : public Shape {
public:
    bool colliding = false;
    std::string id;

public:
    MyCube(std::string id, glm::vec3 pos, Shader* shader, glm::vec3 color) : Shape(shader), id(id) {
//        CollisionHandler::getInstance().subscribe<MyCube>(typeid(MyCube), [this](CollisionData data) {
//            this->onCollision(static_cast<MyCube*>(data.entityA));
//        });

        position = pos;
        this->vertices.push_back({glm::vec3(-0.5f, -0.5f,  0.5f), color, 0.5f});
        this->vertices.push_back({glm::vec3(0.5f, -0.5f,  0.5f), color, 0.5f});
        this->vertices.push_back({glm::vec3(0.5f, 0.5f,  0.5f), color, 0.5f});
        this->vertices.push_back({glm::vec3(-0.5f, 0.5f,  0.5f), color, 0.5f});

        this->vertices.push_back({glm::vec3(-0.5f, -0.5f,  -0.5f), color, 0.5f});
        this->vertices.push_back({glm::vec3(0.5f, -0.5f,  -0.5f), color, 0.5f});
        this->vertices.push_back({glm::vec3(0.5f, 0.5f,  -0.5f), color, 0.5f});
        this->vertices.push_back({glm::vec3(-0.5f, 0.5f,  -0.5f), color, 0.5f});
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
        this->fixed = false;
    }

    void render(glm::mat4 view, glm::mat4 projection) override {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Set up the model matrix
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, this->getPosition());

        // Use the shader program and set uniform values
        this->shader->use();
        this->shader->setMat4("view", view);
        this->shader->setMat4("projection", projection);
        this->shader->setMat4("model", model);

        // Handle debug rendering
        if (Settings::DEBUG_RENDERING) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glPointSize(10.0f);

            glBindVertexArray(this->getVAO());
            glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(vertices.size()));
            glBindVertexArray(0);

//            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        glBindVertexArray(this->getVAO());
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glDisable(GL_BLEND);
    }




    void update(float deltaTime) override {
        Shape::update(deltaTime);
        if(this->colliding) {
            this->setColor(glm::vec3(0.0f, 1.0f, 0.0f));
            this->colliding = false;
        } else {
            this->setColor(glm::vec3(1.0f, 0.0f, 0.0f));
        }
        if(this->onUpdate) {
            this->onUpdate(deltaTime);
        }
    }

    void onCollision(MyCube* entity) {
        std::cout << "Colliding " << this->id << ":" <<  entity->id << std::endl;
        this->colliding = true;
    }

    static std::type_index getType() {
        return typeid(MyCube);
    }
};


#endif //GAMEENGINE_MYCUBE_H
