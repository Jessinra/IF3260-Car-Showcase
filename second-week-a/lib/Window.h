#ifndef WINDOW_H
#define WINDOW_H

#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>

class Window {

private:
  int height, width;  
  const char* title;
  GLFWwindow* window = nullptr;
  
public:
  Window(int width, int height, const char* title);
  void run();
  void initializeGLFW();
  void createWindow();
  void initializeGLAD();
};

#endif