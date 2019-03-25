#include "lib/Window.hpp"
#include <string>
#include "lib/loader.hpp"

#define SCR_WIDTH 800
#define SCR_HEIGHT 600

int main() {
  char* vertexSourceProgram;
  char* fragmentSourceProgram;

  // Testing
  float vertices[] = {
    0.0f, 0.6f, 0.0f,     
    0.65f, 0.10f, 0.0f,   
    0.46f, -0.74f, 0.0f
  };
  float colors[] = {
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f
  };

  try {
    readFile("assets/vectorSource.glsl", &vertexSourceProgram);
    readFile("assets/fragmentSource.glsl", &fragmentSourceProgram);
  } catch (std::string error) {
    printf("ERROR:\n%s\n", error.c_str());
    exit(1);
  }

  Window w(SCR_WIDTH, SCR_HEIGHT, "Duragon");
  try {
    w.initializeGLFW();
    w.createWindow();
    w.initializeGLFW();
    w.initializeGLAD();
    w.createVertexShader(&vertexSourceProgram);
    w.createFragmentShader(&fragmentSourceProgram);
    w.attachToProgram();
    w.deleteShader();
    // Testing
    w.bindBuffer(vertices, colors, 9, 9);
    w.run();
  } catch (std::string error) {
    glfwTerminate();
    printf("ERROR:\n%s\n", error.c_str());
    exit(1);
  } 
}
