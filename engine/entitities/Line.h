#ifndef GAME_ENGINE_LINE_H
#define GAME_ENGINE_LINE_H

#include "../physics/RigidBody.h"
#include "glad/glad.h"
#include "../core/shaders/shader-compiler.h"
#include "../core/shaders/ShaderManager.h"

class Line : public RigidBody {
    glm::vec3 rotation = glm::vec3(0.0f);
public:
    Line(const glm::vec3& start, const glm::vec3& end, glm::vec3 color, float opacity) :
            RigidBody(ShaderManager::getInstance().getShader("shader1")) {
        this->vertices.push_back({start, color, opacity});
        this->vertices.push_back({end, color, opacity});

        indices = { 0, 1 };
        this->flags->set(EntityFlags::ENTITY_FIRST_UPDATE);
        updateVertexBuffer();
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

    int update(float deltaTime) override {
        glm::mat4 model = glm::mat4(1.0f);
//        updateBoundingBoxRegion(model);
        this->shader->setMat4("model", model);
        return 0;
    }

    void setOpacity(float opacity) {
        for(Vertex vertex: vertices){
            vertex.alpha = opacity;
        }
    }

};

#endif //SOFTWAREENGINEERINGPROJECT_LINE_H
