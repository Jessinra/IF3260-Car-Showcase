#include "include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>

#define SCR_WIDTH 800
#define SCR_HEIGHT 600

// Source code program
const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = vec4(0.2f, 0.2f, 0.6f, 0.8f);\n"                 
                                   "}\n\0";

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void initializeGLFW();
GLFWwindow* createWindow();
int finishingError();
int createShader(int SHADER_TYPE, const char* source);

int main() {
    initializeGLFW();
    // Window creation
    GLFWwindow* window = createWindow();
    if (window == NULL) {
        printf("Failed to create GLFW window\n");
        return finishingError();
    }
    // Glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize GLAD\n");
        return finishingError();
    }
    // Build and compile shader program
    int success;
    char infoLog[512];
    int vertexShader, fragmentShader, shaderProgram;
    // Vertex shader
    vertexShader = createShader(GL_VERTEX_SHADER, vertexShaderSource);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("Shader Vertex Failed: %s\n", infoLog);
    }
    // Fragment shader
    fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("Fragment Vertex Failed: %s\n", infoLog);
    }
    // link shaders and attach it
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("Compile Program Failed: %s\n", infoLog);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    // Our vertices and indices
    float vertices[] = {
        0.0f, 0.6f, 0.0f,     // top
        0.65f, 0.10f, 0.0f,   // right
        0.46f, -0.74f, 0.0f,  // bottom right
        -0.46f, -0.74f, 0.0f, // bottom left
        -0.65f, 0.10f, 0.0f   // left
    };
    unsigned int indices[] = {
        0, 1, 4, // top
        1, 2, 3, // bottom left
        3, 4, 1  // bottom right
    };
    int nIndices = sizeof(indices) / sizeof(indices[0]);              // Jessin : define how much indices to render

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    // Render loop window
    while (!glfwWindowShouldClose(window)) {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // Deallocate
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();
    return 0;    
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void initializeGLFW() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

GLFWwindow* createWindow() {
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        return NULL;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    return window;
}

int finishingError() {
    glfwTerminate();
    return -1;
}

int createShader(int SHADER_TYPE, const char *source) {
    int shader = glCreateShader(SHADER_TYPE);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    return shader;
}