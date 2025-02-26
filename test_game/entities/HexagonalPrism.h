/*
// Created by eceva on 1/30/2025.
*/

#ifndef GAMEENGINE_HEXAGONALPRISM_H
#define GAMEENGINE_HEXAGONALPRISM_H

#include <GLFW/glfw3.h>
#include <GL/gl.h>

#include "../../engine/entitities/Shape.h"
#include "../../engine/physics/CollisionHandler.h"
#include "../../engine/entitities/Point.h"
#include "../../engine/core/settings/settings.h"
#include <limits>

class HexagonalPrism : public Shape {
private:

public:
    glm::vec3 color;
    bool colliding = false;
    std::string id;


public:
    HexagonalPrism(std::string id, glm::vec3 pos, glm::vec3 color) : Shape(ShaderManager::getInstance().getShader("shader1")), id(id), color(color) {
        position = pos;

        this->vertices = {
                {glm::vec3(0.0f, -0.5f,  0.0f), color, 1.0f},
                {glm::vec3(0.5f, -0.5f,  0.0f), color, 1.0f},
                {glm::vec3(0.25f, -0.5f,  0.433f), color, 1.0f},
                {glm::vec3(-0.25f, -0.5f,  0.433f), color, 1.0f},
                {glm::vec3(-0.5f, -0.5f,  0.0f), color, 1.0f},
                {glm::vec3(-0.25f, -0.5f, -0.433f), color, 1.0f},
                {glm::vec3(0.25f, -0.5f, -0.433f), color, 1.0f},

                {glm::vec3(0.0f,  0.5f,  0.0f), color, 1.0f},
                {glm::vec3(0.5f,  0.5f,  0.0f), color, 1.0f},
                {glm::vec3(0.25f,  0.5f,  0.433f), color, 1.0f},
                {glm::vec3(-0.25f,  0.5f,  0.433f), color, 1.0f},
                {glm::vec3(-0.5f,  0.5f,  0.0f), color, 1.0f},
                {glm::vec3(-0.25f,  0.5f, -0.433f), color, 1.0f},
                {glm::vec3(0.25f,  0.5f, -0.433f), color, 1.0f},
        };


        indices = {
                0, 1, 2,
                0, 2, 3,
                0, 3, 4,
                0, 4, 5,
                0, 5, 6,
                0, 6, 1,

                7, 8, 9,
                7, 9, 10,
                7, 10, 11,
                7, 11, 12,
                7, 12, 13,
                7, 13, 8,

                1, 8, 9,
                1, 9, 2,
                2, 9, 10,
                2, 10, 3,
                3, 10, 11,
                3, 11, 4,
                4, 11, 12,
                4, 12, 5,
                5, 12, 13,
                5, 13, 6,
                6, 13, 8,
                6, 8, 1
        };
//        updateVertexBuffer();
        initializeBuffers();
        this->flags->set(EntityFlags::ENTITY_FIRST_UPDATE);
        updateBoundingBoxRegion(model);
        this->fixed = false;

    }

    glm::vec3 getSupportPoint(const glm::vec3& direction) {
        float maxDot = -std::numeric_limits<float>::infinity();
        glm::vec3 supportPoint;

        for (const auto& vertex : vertices) {
            glm::vec3 transformedVertex = position + vertex.position;
            float dotProduct = glm::dot(transformedVertex, direction);
            if (dotProduct > maxDot) {
                maxDot = dotProduct;
                supportPoint = transformedVertex;
            }
        }

        return supportPoint;
    }

    void render(glm::mat4 view, glm::mat4 projection) override {

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        this->model = glm::mat4(1.0f);
        model = glm::translate(model, this->getPosition());
        model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, scale);

        this->shader->use();
        this->shader->setMat4("view", view);
        this->shader->setMat4("projection", projection);
        this->shader->setMat4("model", model);

        glBindVertexArray(this->getVAO());
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glDisable(GL_BLEND);
//        if(boundingRegion) boundingRegion->render(view, projection);
    }

    int update(float deltaTime) override {
        bool dirty = false;
        dirty &= Shape::update(deltaTime);
        dirty &= this->onUpdate(deltaTime);
        this->updateBoundingBoxRegion(model);
        if (this->colliding) {
            this->setColor(glm::vec3(0.0f, 1.0f, 0.0f));
            this->colliding = false;
        } else {
            this->setColor(color);
        }
        return dirty;
    }

    void onCollision(HexagonalPrism* entity) {
        std::cout << "Colliding " << this->id << ":" << entity->id << std::endl;
        this->colliding = true;
    }

    static std::type_index getType() {
        return typeid(HexagonalPrism);
    }
};

#endif //GAMEENGINE_HEXAGONALPRISM_H
