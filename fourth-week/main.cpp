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

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void setupWindow(GLFWwindow **window);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);

glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
glm::mat4 view;
glm::mat4 model = glm::mat4(1.0);

/* ===========================================
                Camera section 
=========================================== */

Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));
bool firstMouse = true;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

/* ===========================================
                Lighting section 
=========================================== */

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main(int argc, char ** argv){

    GLFWwindow *window = NULL;

    try {

        setupWindow(&window);
        
        // glad: load all OpenGL function pointers
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
            throw 2;
        }

        // Create texture shader
        Shader textureShader("assets/vertexSource.glsl", "assets/fragmentSource.glsl");
        textureShader.use();
        textureShader.setMat4("projection", projection);
        textureShader.setMat4("model", model);
        Model model("assets/jimny.obj");

        // Lighting shader
        Shader lightingShader("1.colors.vs", "1.colors.fs");
        Shader lampShader("1.lamp.vs", "1.lamp.fs");

        /* ===========================================
                Draw Object Section 
        =========================================== */
        
        unsigned int carVAO;
        GLuint points_VBO, tex_VBO;

        glGenVertexArrays(1, &carVAO);
        glGenBuffers(1, &points_VBO);
        glGenBuffers(1, &tex_VBO);

        glBindBuffer(GL_ARRAY_BUFFER, points_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * model.getConstRefPoints().size(), model.getConstRefPoints().data(), GL_STATIC_DRAW);
        glBindVertexArray(carVAO);  // note : must happend before glVertexAttribPointer
        
        // object position attribute        
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

         // normal attribute
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, tex_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * model.getConstRefTextures().size(), model.getConstRefTextures().data(), GL_STATIC_DRAW);
        glBindVertexArray(carVAO);  // note : must happend before glVertexAttribPointer
       
        // texture position attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);

        glEnable(GL_DEPTH_TEST);

        /* ===========================================
                    Lighting Section 
        =========================================== */

        unsigned int lightVAO;
        glGenVertexArrays(1, &lightVAO);
        glBindVertexArray(lightVAO);
        
        glBindBuffer(GL_ARRAY_BUFFER, points_VBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        /* ===========================================
                    Handle Texture Section 
        =========================================== */

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

        glUniform1i(glGetUniformLocation(textureShader.StackedShader, "tex_sampler"), 0);

        // Render loop
        while (!glfwWindowShouldClose(window)) {

            glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            processInput(window);

            // apply lighting shader
            lightingShader.use();
            // lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
            // lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
            lightingShader.setVec3("lightPos", lightPos);
            lightingShader.setVec3("viewPos", camera.Position);

            // light properties
            glm::vec3 lightColor;

            lightColor.x = sin(glfwGetTime() * 2.0f);
            lightColor.y = sin(glfwGetTime() * 0.7f);
            lightColor.z = sin(glfwGetTime() * 1.3f);

            glm::vec3 diffuseColor = lightColor   * glm::vec3(0.5f); // decrease the influence
            glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence
            
            lightingShader.setVec3("light.ambient", ambientColor);
            lightingShader.setVec3("light.diffuse", diffuseColor);
            lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

            // material properties
            lightingShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
            lightingShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
            lightingShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f); // specular lighting doesn't have full effect on this object's material
            lightingShader.setFloat("material.shininess", 32.0f);

            // apply camera trasnformations
            glm::mat4 view = camera.GetViewMatrix();
            lightingShader.setMat4("projection", projection);
            lightingShader.setMat4("view", view);

            // apply texture shader
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);

            // apply camera trasnformations
            textureShader.setMat4("projection", projection);
            textureShader.setMat4("view", view);

            // render the object
            glBindVertexArray(carVAO);
            glDrawArrays(GL_TRIANGLES, 0, model.getConstRefPoints().size());
            
            // world transformation
            glm::mat4 lampModel = glm::mat4(1.0f);
            lightingShader.setMat4("model", lampModel);

            glBindVertexArray(lightVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            glfwSwapBuffers(window);
            glfwPollEvents();

        }

        glDeleteVertexArrays(1, &carVAO);
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

    glfwMakeContextCurrent(*window);
    glfwSetFramebufferSizeCallback(*window, framebuffer_size_callback);
    glfwSetCursorPosCallback(*window, mouseCallback);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window){
    
    float cameraSpeed = 0.01f; 

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.moveOnKeyPress(Camera_Movement::FORWARD, cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.moveOnKeyPress(Camera_Movement::BACKWARD, cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.moveOnKeyPress(Camera_Movement::LEFT, cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.moveOnKeyPress(Camera_Movement::RIGHT, cameraSpeed);
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