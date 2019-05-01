#include "modules/glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include "lib/Shader.hpp"
#include "lib/Camera.hpp"
#include "objects/Car.hpp"
#include "objects/Lamp.hpp"
#include "objects/Rain.hpp"

#define SCR_WIDTH 900
#define SCR_HEIGHT 600
#define TARGET_FPS 60

using namespace std;

const glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
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

        // Objects
        Car car(projection, camera);
        Lamp lamp(projection, camera);

        // Particles
        const size_t maxParticles = 1000;
        Rain rain(projection, camera, maxParticles);

        glEnable(GL_DEPTH_TEST);

        bool right = true;
        // Render loop
        double startTime = glfwGetTime();
        double printTime = startTime;
        while (!glfwWindowShouldClose(window)) {
            glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            processInput(window);

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

            car.render(lightPos);

            // Draw lamp
            lamp.render(lightPos);

            // Draw rain
            rain.render(lightPos);

            glfwSwapBuffers(window);
            double doneTime = glfwGetTime();
            if ((doneTime - printTime) > 1) {
                printTime = doneTime;
                cout << (1 / (doneTime - startTime)) << endl;
            }
            startTime = doneTime;
            glfwPollEvents();
        }
        
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
    // finally
    glfwTerminate();
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
