#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>

class Window {

private:
  int height, width;
  bool running = true;

  int vertexShader, fragmentShader;
  int shaderProgram;  
  unsigned int VAO, VBOPoints, VBOColors;
  
  int success, size;
  const char* title;
  char info[512];
  GLFWwindow* window = nullptr;
  
public:
  Window(int width, int height, const char* title);
  void initializeGLFW();
  void createWindow();
  void initializeGLAD();
  void createVertexShader(char** vertexShaderSource);
  void createFragmentShader(char** fragmentShaderSource);
  void attachToProgram();
  void deleteShader();
  void bindBuffer(float* vertices, float* colors, int vertexCount, int colorCount);
  void run();
};

#endif