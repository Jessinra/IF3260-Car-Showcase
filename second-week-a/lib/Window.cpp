#include "Window.hpp"
#include "loader.hpp"
#include <string>
#include <stdio.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

Window::Window(int width, int height, const char* title) : title(title) {
  this->height = height;
  this->width = width;
}

void Window::initializeGLFW() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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

void Window::bindBuffer() {
  for (auto object: objects) {
    object->bind();
  }
}

void Window::addObject(BaseObject *obj) {
  objects.push_back(obj);
}

void Window::run() {
  while (!glfwWindowShouldClose(window)) {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shaderProgram);
    for (auto object: objects) {
      object->draw();
    }
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  for (auto object: objects) {
    delete object;
  }
  glfwTerminate();
}
