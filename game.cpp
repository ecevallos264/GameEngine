#include "game.h"
#include "camera.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"

void Game::processInput(GLFWwindow *window, float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        Camera::getInstance().setPosition(Camera::getInstance().getPosition() + (Camera::getInstance().getSpeed(deltaTime) * Camera::getInstance().getFront()));
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        Camera::getInstance().setPosition(Camera::getInstance().getPosition() - (Camera::getInstance().getSpeed(deltaTime) * Camera::getInstance().getFront()));
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        Camera::getInstance().setPosition(Camera::getInstance().getPosition() - glm::normalize(glm::cross(Camera::getInstance().getFront(), Camera::getInstance().getUp())) * Camera::getInstance().getSpeed(deltaTime));
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        Camera::getInstance().setPosition(Camera::getInstance().getPosition() + glm::normalize(glm::cross(Camera::getInstance().getFront(), Camera::getInstance().getUp())) * Camera::getInstance().getSpeed(deltaTime));
}

void Game::renderGame(GLFWwindow* window) {
    this->shader->use();

    glm::mat4 view = glm::lookAt(
            Camera::getInstance().getPosition(),
            Camera::getInstance().getPosition() + Camera::getInstance().getFront(),
            Camera::getInstance().getUp()
    );
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);

    shader->setMat4("view", view);
    shader->setMat4("projection", projection);

    glm::vec3 hexagonPositions[] = {
            glm::vec3(0.0f, 0.0f, 0.0f),   // Center
//            glm::vec3(2.0f, 0.0f, 0.0f),   // Right
//            glm::vec3(-2.0f, 0.0f, 0.0f),  // Left
//            glm::vec3(0.0f, 2.0f, 0.0f),   // Top
//            glm::vec3(0.0f, -2.0f, 0.0f),  // Bottom
            glm::vec3(2.0f, 2.0f, 0.0f)    // Diagonal
    };

    glBindVertexArray(Game::getInstance().VAO);

    for (int i = 0; i < sizeof (hexagonPositions) / sizeof (glm::vec3); i++) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, hexagonPositions[i]); // Move each hexagon to its position
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(1.0f, 0.0f, 1.0f)); // Optional rotation
        shader->setMat4("model", model);

//        glDrawArrays(GL_TRIANGLES, 0, 36 / (6 * sizeof(float))); // Adjust vertex count if needed
//        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDrawElements(GL_TRIANGLES, Game::getInstance().indices.size(), GL_UNSIGNED_INT, 0);
    }

    glBindVertexArray(0);
}

void Game::setVertices(const std::vector<float>& v) {
    vertices = v;
}

void Game::setIndices(const std::vector<unsigned int> &v) {
    indices = v;
}

void Game::run(GLFWwindow* window) {
    this->shader->setVec3("shapeColor", glm::vec3(1.0f, 0.0f, 0.0f));

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window, deltaTime);
        std::cout << deltaTime << std::endl;
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderGame(window);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}