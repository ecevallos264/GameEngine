#include "game.h"
#include "camera.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "shapes/cube.h"
#include "shapes/hex-prism.h"

void Game::processInput(GLFWwindow *window, float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && !Game::getInstance().shiftPressed) {
        Camera::getInstance().setSpeed(Camera::getInstance().getSpeed() * 2);
        Game::getInstance().shiftPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE && Game::getInstance().shiftPressed) {
        Camera::getInstance().setSpeed(Camera::getInstance().getSpeed() / 2);
        Game::getInstance().shiftPressed = false;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        Camera::getInstance().setPosition(Camera::getInstance().getPosition() + (Camera::getInstance().getCalcSpeed(deltaTime) * Camera::getInstance().getFront()));
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        Camera::getInstance().setPosition(Camera::getInstance().getPosition() - (Camera::getInstance().getCalcSpeed(deltaTime) * Camera::getInstance().getFront()));
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        Camera::getInstance().setPosition(Camera::getInstance().getPosition() - glm::normalize(glm::cross(Camera::getInstance().getFront(), Camera::getInstance().getUp())) * Camera::getInstance().getCalcSpeed(deltaTime));
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        Camera::getInstance().setPosition(Camera::getInstance().getPosition() + glm::normalize(glm::cross(Camera::getInstance().getFront(), Camera::getInstance().getUp())) * Camera::getInstance().getCalcSpeed(deltaTime));
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        Camera::getInstance().setPosition(Camera::getInstance().getPosition() + glm::vec3(0.0f, Camera::getInstance().getCalcSpeed(deltaTime), 0.0f));
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

//    glm::vec3 hexagonPositions[] = {
//            glm::vec3(0.0f, 0.0f, 0.0f),   // Center
//            glm::vec3(2.0f, 0.0f, 0.0f),   // Right
//            glm::vec3(-2.0f, 0.0f, 0.0f),  // Left
//            glm::vec3(0.0f, 2.0f, 0.0f),   // Top
//            glm::vec3(0.0f, -2.0f, 0.0f),  // Bottom
//            glm::vec3(2.0f, 2.0f, 0.0f)    // Diagonal
//    };

    for (auto &shape: items) {
        unsigned int VBO, EBO;
        glGenVertexArrays(1, &Game::getInstance().VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(Game::getInstance().VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, shape->getVertices().size() * sizeof(float), shape->getVertices().data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, shape->getIndices().size() * sizeof(unsigned int), shape->getIndices().data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, shape->getPosition());
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(1.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(1.0f, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 1.0f));

        shader->setMat4("model", model);

        glDrawElements(GL_TRIANGLES, shape->getIndices().size(), GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteVertexArrays(1, &Game::getInstance().VAO);
    }
}

void Game::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    Camera::getInstance().yaw   += xoffset;
    Camera::getInstance().pitch += yoffset;

    if(Camera::getInstance().pitch > 89.0f)
        Camera::getInstance().pitch = 89.0f;
    if(Camera::getInstance().pitch < -89.0f)
        Camera::getInstance().pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(Camera::getInstance().yaw)) * cos(glm::radians(Camera::getInstance().pitch));
    direction.y = sin(glm::radians(Camera::getInstance().pitch));
    direction.z = sin(glm::radians(Camera::getInstance().yaw)) * cos(glm::radians(Camera::getInstance().pitch));
    Camera::getInstance().setFront(glm::normalize(direction));
}

void Game::setVertices(const std::vector<float>& v) {
    vertices = v;
}

void Game::setIndices(const std::vector<unsigned int> &v) {
    indices = v;
}

void Game::run(GLFWwindow* window) {
    this->shader->setVec3("shapeColor", glm::vec3(1.0f, 0.0f, 0.0f));

    this->items.push_back(new Cube(glm::vec3(0.0f, 0.0f, 0.0f)));
    this->items.push_back(new HexagonalPrism(glm::vec3(2.0f, 0.0f, 0.0f)));

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window, deltaTime);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderGame(window);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}