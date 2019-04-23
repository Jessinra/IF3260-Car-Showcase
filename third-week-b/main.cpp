#include "modules/glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "model/Model.hpp"
#include "lib/Shader.hpp"
#include "lib/Camera.hpp"

#define SCR_WIDTH 900
#define SCR_HEIGHT 600

using namespace std;

glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
glm::mat4 view;
glm::mat4 model = glm::mat4(1.0);
Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));

bool firstMouse = true;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void setupWindow(GLFWwindow **window);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);

int main(int argc, char ** argv){
    GLFWwindow *window = NULL;
    try {
        setupWindow(&window);
        // glad: load all OpenGL function pointers
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
            throw 2;
        }
        Shader shader("assets/vertexSource.glsl", "assets/fragmentSource.glsl");
        shader.use();
        shader.setMat4("projection", projection);
        shader.setMat4("model", model);
        Model model("assets/jimny.obj");

        unsigned int VAO;
        GLuint points_VBO, tex_VBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &points_VBO);
        glGenBuffers(1, &tex_VBO);
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, points_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * model.getConstRefPoints().size(), model.getConstRefPoints().data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

        glBindBuffer(GL_ARRAY_BUFFER, tex_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * model.getConstRefTextures().size(), model.getConstRefTextures().data(), GL_STATIC_DRAW);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(0);
        glEnable(GL_DEPTH_TEST);

        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true); // Tell stb_image.h to flip loaded texture's on the y-axis.
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load image, create texture and generate mipmaps
        stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
        // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
        unsigned char *data = stbi_load("assets/wall.jpg", &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            std::cout << "Failed to load texture" << std::endl;
            exit(1);
        }
        stbi_image_free(data);

        glUniform1i(glGetUniformLocation(shader.StackedShader, "tex_sampler"), 0);

        // Render loop
        while (!glfwWindowShouldClose(window)) {
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            processInput(window);

            glm::mat4 view = camera.GetViewMatrix();
            shader.setMat4("view", view);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);

            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, model.getConstRefPoints().size());

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &points_VBO);
        glDeleteBuffers(1, &tex_VBO);
        glfwTerminate();

    } catch (int val) {
        
        glfwTerminate();

        if (val == 1){
            cout << "Failed to create GLFW window" << endl;
            return -1;

        } else if (val == 2) {
            cout << "Failed to initialize GLAD" << endl;
            return -1;
        }
    }
    return 0;
}

void setupWindow(GLFWwindow **window){
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Tugas JEEP", NULL, NULL);
    if (window == NULL){
        throw 1;
    }

    glfwSetFramebufferSizeCallback(*window, framebuffer_size_callback);
    // glfwSetCursorPosCallback(*window, mouseCallback);
    glfwMakeContextCurrent(*window);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window){
    float cameraSpeed = 0.003f; 
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.moveOnKeyPress(Camera_Movement::FORWARD, cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.moveOnKeyPress(Camera_Movement::BACKWARD, cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.moveOnKeyPress(Camera_Movement::LEFT, cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.moveOnKeyPress(Camera_Movement::RIGHT, cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        camera.moveOnKeyPress(Camera_Movement::UP, cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        camera.moveOnKeyPress(Camera_Movement::DOWN, cameraSpeed);
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    camera.moveOnMouseMovement(xoffset, yoffset);
}