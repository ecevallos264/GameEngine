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
#include "entities/sphere.h"
#include "entities/particle.h"
#include "my_math.h"
#include "Entity_Handler.h"
#include "externals/imgui/imgui.h"
#include "externals/imgui/imgui_impl_opengl3.h"
#include "externals/imgui/imgui_impl_glfw.h"
#include "entities/vector.h"
#include "gui.h"


void Game::processInput(GLFWwindow *window, float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        Settings::getInstance().CURSOR_FOCUS_STATUS = true;
    }

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
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
        for(int i = 0; i < 10; i++) {
            Shape* shape = new Particle(glfwGetTime(), glm::vec3(0.0,0.0,0.0), generateRandomLocation(-1,1.0,-1.0,1.0,-1.0,1.0));
            EntityHandler::getInstance().addEntity(shape);
        }
    }
}

void Game::renderGame(GLFWwindow* window) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLineWidth(2.0f); // Set line width
    this->shader->use();

    glm::mat4 view = glm::lookAt(
            Camera::getInstance().getPosition(),
            Camera::getInstance().getPosition() + Camera::getInstance().getFront(),
            Camera::getInstance().getUp()
    );
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, Settings::getInstance().MAX_RENDER_DISTANCE);

    shader->setMat4("view", view);
    shader->setMat4("projection", projection);

    for(int i = 0; i < EntityHandler::getInstance().getEntities().size(); i++) {
        if(EntityHandler::getInstance().entities[i]->isDestroyed()) {
            EntityHandler::getInstance().entities.erase(EntityHandler::getInstance().entities.begin() + i);
        }
        EntityHandler::getInstance().entities[i]->update(deltaTime, shader);
        EntityHandler::getInstance().entities[i]->draw();
    }
}

void Game::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

    bool cursorInWindow = (xpos >= 0 && xpos <= WINDOW_WIDTH && ypos >= 0 && ypos <= WINDOW_HEIGHT);

    if (!Settings::getInstance().CURSOR_FOCUS_STATUS) {
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }
        this->xPos = xpos;
        this->yPos = ypos;

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;
        lastX = xpos;
        lastY = ypos;

        float sensitivity = Settings::getInstance().CURSOR_SENSITIVITY;
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
    } else {
        firstMouse = true;
    }
}

void Game::renderGrid() {
    float unit = 1.0f;
    for(float i = -Settings::getInstance().MAX_RENDER_DISTANCE * 1/unit; i <= Settings::getInstance().MAX_RENDER_DISTANCE * 1/unit; i++) {
        if(i == 0) continue;
        float line = i * unit;
        EntityHandler::getInstance().addEntity(new Line(glm::vec3(Settings::getInstance().MAX_RENDER_DISTANCE, 0.0f, line), glm::vec3(-Settings::getInstance().MAX_RENDER_DISTANCE, 0.0f, line), glm::vec3(0.5f, 0.5f, 0.5f), 1.0f));
    }

    for(float i = -Settings::getInstance().MAX_RENDER_DISTANCE * 1/unit; i <= Settings::getInstance().MAX_RENDER_DISTANCE * 1/unit; i++) {
        if(i == 0) continue;
        float line = i * unit;
        EntityHandler::getInstance().addEntity(new Line(glm::vec3(line, 0.0f,Settings::getInstance().MAX_RENDER_DISTANCE), glm::vec3(line, 0.0f, -Settings::getInstance().MAX_RENDER_DISTANCE), glm::vec3(0.5f, 0.5f, 0.5f), 1.0f));
    }
}

void Game::run(GLFWwindow* window) {
    this->shader->setVec3("shapeColor", glm::vec3(1.0f, 0.0f, 0.0f));

    renderGrid();
    EntityHandler::getInstance().addEntity(new Line(glm::vec3(Settings::getInstance().MAX_RENDER_DISTANCE, 0.0f, 0.0f), glm::vec3(-Settings::getInstance().MAX_RENDER_DISTANCE, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), 1.0f));
    EntityHandler::getInstance().addEntity(new Line(glm::vec3(0.0f, Settings::getInstance().MAX_RENDER_DISTANCE, 0.0f), glm::vec3(0.0f, -Settings::getInstance().MAX_RENDER_DISTANCE, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 1.0f));
    EntityHandler::getInstance().addEntity(new Line(glm::vec3(0.0f, 0.0f, Settings::getInstance().MAX_RENDER_DISTANCE), glm::vec3(0.0f, 0.0f, -Settings::getInstance().MAX_RENDER_DISTANCE), glm::vec3(0.0f, 0.0f, 1.0f), 1.0f));

    EntityHandler::getInstance().addEntity(new Cube(glm::vec3(-2.0f, 0.0f, -2.0f)));
    EntityHandler::getInstance().addEntity(new HexagonalPrism(glm::vec3(2.0f, 0.0f, 0.0f)));
    EntityHandler::getInstance().addEntity(new TriangularPrism(glm::vec3(2.0f, 0.0f, 2.0f)));
    EntityHandler::getInstance().addEntity(new Pyramid(glm::vec3(2.0f, 2.0f, 2.0f)));
    EntityHandler::getInstance().addEntity(new Sphere(glm::vec3(-1.0f, 2.0f, 1.0f), 1.0f, 100, 0.5f));




    shader->use();
    GUI* gui = new GUI();
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        processInput(window, deltaTime);
        renderGame(window);

        gui->render(window);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}