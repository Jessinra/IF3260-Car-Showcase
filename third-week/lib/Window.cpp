#include "Window.hpp"
#include <string>
#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

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
  glEnable(GL_DEPTH_TEST);
}

void Window::bindBuffer() {
  for (auto object: objects) {
    object->bind();
  }
}

void Window::bindTexture(Shader& shader) {
  glGenTextures(1, &texture);
  // glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture); 
    // set the texture wrapping parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // load image, create texture and generate mipmaps
  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
  // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
  unsigned char *data = stbi_load("assets/wall.jpg", &width, &height, &nrChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    throw "Failed to load texture";
  }
  stbi_image_free(data);
  shader.use();
  glUniform1i(glGetUniformLocation(shader.ID, "texture_sampler"), 0);
}

void Window::addObject(Mesh *obj) {
  objects.push_back(obj);
}

void Window::run(Shader& shader, Camera& camera, glm::mat4& view) {

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    processInput(camera);

    view = glm::mat4(1.0f);
    view = camera.GetViewMatrix();
    shader.setMat4("view", view);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    //shader.use();
    
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

void Window::processInput(Camera& camera) {
  float cameraSpeed = 0.03f; // adjust accordingly 
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    camera.ProcessKeyboard(Camera_Movement::FORWARD, cameraSpeed); 
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) 
    camera.ProcessKeyboard(Camera_Movement::BACKWARD, cameraSpeed); 
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) 
    camera.ProcessKeyboard(Camera_Movement::LEFT, cameraSpeed); 
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.ProcessKeyboard(Camera_Movement::RIGHT, cameraSpeed); 
}