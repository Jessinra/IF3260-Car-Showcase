#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "Shader.hpp"
#include "Mesh.hpp"
#include "Camera.hpp"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

class Window {
public:
  int height, width;
  bool running = true;
  int success;
  const char* title;
  GLFWwindow* window = nullptr;  
  std::vector<Mesh*> objects;
  unsigned int texture;

  Window(int width, int height, const char* title);
  void initializeGLFW();
  void createWindow();
  void initializeGLAD();
  void bindBuffer();
  void bindTexture(Shader& s);
  void run(Shader& s, Camera& camera, glm::mat4& view);
  void addObject(Mesh *obj);
  void processInput(Camera& camera);
};

#endif