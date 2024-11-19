#include "game.h"
#include "camera.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "entities/cube.h"
#include "entities/hex-prism.h"
#include "entities/triangular-prism.h"
#include "entities/pyramid.h"
#include "entities/line.h"
#include "entities/xPlane.h"


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
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, maxRenderDistance);

    shader->setMat4("view", view);
    shader->setMat4("projection", projection);

    for (const auto &shape : items) {



//        model = glm::rotate(model, (float)time * glm::radians(50.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        shape->update(deltaTime, shader);
        shape->draw();


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

void Game::run(GLFWwindow* window) {
    this->shader->setVec3("shapeColor", glm::vec3(1.0f, 0.0f, 0.0f));



    this->items.push_back(new Cube(glm::vec3(0.0f, 0.0f, 0.0f)));
    this->items.push_back(new HexagonalPrism(glm::vec3(2.0f, 0.0f, 0.0f)));
    this->items.push_back(new TriangularPrism(glm::vec3(2.0f, 0.0f, 2.0f)));
    this->items.push_back(new Pyramid(glm::vec3(2.0f, 2.0f, 2.0f)));

    this->items.push_back(new Line(glm::vec3(maxRenderDistance, 0.0f, 0.0f), glm::vec3(-maxRenderDistance, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    this->items.push_back(new Line(glm::vec3(0.0f, maxRenderDistance, 0.0f), glm::vec3(0.0f, -maxRenderDistance, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    this->items.push_back(new Line(glm::vec3(0.0f, 0.0f, maxRenderDistance), glm::vec3(0.0f, 0.0f, -maxRenderDistance), glm::vec3(0.0f, 0.0f, 1.0f)));

    this->items.push_back(new xPlane(glm::vec3(0.0f, 0.0f, 0.0f), maxRenderDistance*2, maxRenderDistance*2, glm::vec3(0.53f, 0.53f, 0.53f), 100));
    for(auto &shape : items) {
        shape->initializeBuffers();
    }
    shader->use();
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window, deltaTime);


        renderGame(window);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}