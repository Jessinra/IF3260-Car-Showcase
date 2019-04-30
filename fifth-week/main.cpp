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
Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));

glm::vec3 lightPos(0.0f, 4.0f, 0.0f);

bool firstMouse = true;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void setupWindow(GLFWwindow **window);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);

int main(int argc, char ** argv) {
    GLFWwindow *window = NULL;
    try {
        setupWindow(&window);
        // glad: load all OpenGL function pointers
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
            throw 2;
        }
        // Normal Shader
        Shader shader("assets/vertexSource.glsl", "assets/fragmentSource.glsl");
        // Lamp Shader
        Shader lampShader("assets/vertexLampSource.glsl", "assets/fragmentLampSource.glsl");

        // Model
        Model obj("assets/jimny.obj");
        // Lamp
        float lampVertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
            0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
        };

        unsigned int car_VAO, lamp_VAO;
        unsigned int points_VBO, tex_VBO, norm_VBO, lamp_VBO;
        glGenVertexArrays(1, &car_VAO);
        glGenVertexArrays(1, &lamp_VAO);
        glGenBuffers(1, &points_VBO);
        glGenBuffers(1, &tex_VBO);
        glGenBuffers(1, &norm_VBO);
        glGenBuffers(1, &lamp_VBO);

        glBindVertexArray(car_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, points_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * obj.getConstRefPoints().size(), obj.getConstRefPoints().data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

        glBindBuffer(GL_ARRAY_BUFFER, tex_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * obj.getConstRefTextures().size(), obj.getConstRefTextures().data(), GL_STATIC_DRAW);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

        glBindBuffer(GL_ARRAY_BUFFER, norm_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * obj.getConstRefNormals().size(), obj.getConstRefNormals().data(), GL_STATIC_DRAW);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        glBindVertexArray(lamp_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, lamp_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(lampVertices), lampVertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

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

        bool right = true;
        // Render loop
        while (!glfwWindowShouldClose(window)) {
            glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            processInput(window);

            glm::mat4 view = camera.GetViewMatrix();
            glm::mat4 model = glm::mat4(1.0);
            
            if (lightPos.x > 1.0f) {
                right = false;
            } else if (lightPos.x < -1.0f) {
                right = true;
            }

            if (right) {
                lightPos = glm::vec3(lightPos.x + 0.001f, lightPos.y, lightPos.z);
            } else {
                lightPos = glm::vec3(lightPos.x - 0.001f, lightPos.y, lightPos.z);
            }

            // Set shader obj again
            shader.use();
            shader.setVec3("light_color", 1.0f, 1.0f, 1.0f);
            shader.setVec3("light_pos", lightPos);
            shader.setVec3("view_pos", camera.Position);
            shader.setMat4("projection", projection);
            shader.setMat4("model", model);
            shader.setMat4("view", view);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);

            glBindVertexArray(car_VAO);
            glDrawArrays(GL_TRIANGLES, 0, obj.getConstRefPoints().size());

            // Set shader lamp again
            lampShader.use();
            lampShader.setMat4("projection", projection);
            lampShader.setMat4("view", view);
            glm::mat4 lampModel = glm::mat4(1.0f);
            lampModel = glm::translate(lampModel, lightPos);
            lampModel = glm::scale(lampModel, glm::vec3(0.3f)); // a smaller cube
            lampShader.setMat4("model", lampModel);

            glBindVertexArray(lamp_VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        glDeleteVertexArrays(1, &car_VAO);
        glDeleteVertexArrays(1, &lamp_VAO);
        glDeleteBuffers(1, &points_VBO);
        glDeleteBuffers(1, &tex_VBO);
        glDeleteBuffers(1, &norm_VBO);
        glDeleteBuffers(1, &lamp_VBO);
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
    glfwSwapInterval(1);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window){
    float cameraSpeed = 0.05f;
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
