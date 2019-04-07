#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "BaseObject.hpp"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

class Window {
private:
  int height, width;
  bool running = true;

  int vertexShader, fragmentShader;
  int shaderProgram;  
  
  int success;
  const char* title;

  char info[512];
  GLFWwindow* window = nullptr;

  glm::quat Quaternion = glm::quat(glm::vec3(0, 0, 0));
  
  std::vector<BaseObject*> objects;

public:
  Window(int width, int height, const char* title);
  void initializeGLFW();
  void createWindow();
  void initializeGLAD();
  void createVertexShader(char** vertexShaderSource);
  void createFragmentShader(char** fragmentShaderSource);
  void attachToProgram();
  void deleteShader();
  void bindBuffer();
  void run();
  void addObject(BaseObject *obj);
  void bindKeyCallback();
};

#endif