#include <iostream>
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <filesystem>
#include "shader-compiler.h"
#include "stb_image.h"

#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH 800

float vertices[] = {
        // positions                                             // colors                                              // texture coords
        -0.5f, -0.5f, 0.0f,            1.0f, 0.0f, 0.0f,             0.0f, 0.0f,
        0.5f, -0.5f, 0.0f,          0.0f, 1.0f, 0.0f,       1.0f, 0.0f,
        0.5f,  0.5f, 0.0f,       0.0f, 0.0f, 1.0f,       1.0f, 1.0f,
        0.5f,  0.5f, 0.0f,       0.0f, 0.0f, 1.0f,       1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f,     1.0f, 1.0f, 0.0f,       0.0f, 1.0f,
        -0.5f, -0.5f, 0.0f,    1.0f, 0.0f, 0.0f,       0.0f, 0.0f
};

void renderGame(GLFWwindow* window, Shader &shader, unsigned int VAO, unsigned int VBO) {
    shader.use();
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
//    glBindTexture(GL_TEXTURE, texture);
    glBindVertexArray(0);
}

void setup() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

int main() {
    setup();
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Open GL Learning Material", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    std::filesystem::path contentRoot = std::filesystem::current_path().parent_path();
    std::filesystem::path vertexShaderPath = contentRoot / "shaders/shader.vert";
    std::filesystem::path fragmentShaderPath = contentRoot / "shaders/shader.frag";

    std::cout << "Vertex Shader Path: " << vertexShaderPath << std::endl;
    std::cout << "Fragment Shader Path: " << fragmentShaderPath << std::endl;

    if (!std::filesystem::exists(vertexShaderPath) || !std::filesystem::exists(fragmentShaderPath)) {
        std::cerr << "Shader files not found in the specified directory." << std::endl;
        return -1;
    }

    Shader shader(vertexShaderPath.string().c_str(), fragmentShaderPath.string().c_str());

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); // Color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); // TexCoord
    glEnableVertexAttribArray(2);

    // ******
    unsigned int texture1, texture2;

    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    std::filesystem::path containerPath = contentRoot / "textures/container.jpg";
    std::filesystem::path awesomefacePath = contentRoot / "textures/awesomeface.png";
    stbi_set_flip_vertically_on_load(true);

    unsigned char *data = stbi_load(containerPath.string().c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);



    data = stbi_load(awesomefacePath.string().c_str(), &width, &height, &nrChannels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    shader.use();

    glUniform1i(glGetUniformLocation(shader.ID, "texture1"), 0);

    shader.setInt("texture2", 1);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        renderGame(window, shader, VAO, VBO);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}