#include "Window.hpp"
#include "loader.hpp"
#include <string>

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

Window::Window(int width, int height, const char* title) : title(title) {
  this->height = height;
  this->width = width;
}

void Window::initializeGLFW() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void Window::createWindow() {
  window = glfwCreateWindow(this->width, this->height, title, NULL, NULL);
  if (window == nullptr) {
    throw "error creating window";
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

void Window::initializeGLAD() {
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    throw "Failed to initialize GLAD";
  }
}

void Window::createVertexShader(char** vertexShaderSource) {
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, info);
    std::string infoString(info);
    throw "Vertex Shader Failed: \n" + infoString;
  }
}

void Window::createFragmentShader(char** fragmentShaderSource) {
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, info);
    std::string infoString(info);
    throw "Fragment Shader Failed: \n" + infoString;
  }
}

void Window::attachToProgram() {
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glBindAttribLocation(shaderProgram, 0, "vertex_position");
  glBindAttribLocation(shaderProgram, 1, "vertex_colour");
  glLinkProgram(shaderProgram);
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, info);
    std::string infoString(info);
    throw "Program Linking Failed: \n" + infoString;
  }
}

void Window::deleteShader() {
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

void Window::bindBuffer(float* vertices, float* colors, int vsize, int csize) {
  this->size = vsize;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBOPoints);
  glGenBuffers(1, &VBOColors);
  glBindVertexArray(VAO);
  // Bind vertices
  glBindBuffer(GL_ARRAY_BUFFER, VBOPoints);
  glBufferData(GL_ARRAY_BUFFER, 3 * vsize * sizeof(float), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  // Bind colors
  glBindBuffer(GL_ARRAY_BUFFER, VBOColors);
  glBufferData(GL_ARRAY_BUFFER, 3 * csize * sizeof(float), colors, GL_STATIC_DRAW);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  // Enable vertex attrib
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  // Bind buffer
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void Window::run() {
  while (!glfwWindowShouldClose(window)) {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, this->size);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBOPoints);
  glDeleteBuffers(1, &VBOColors);
  glfwTerminate();
}