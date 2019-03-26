#include "lib/Window.hpp"
#include <string>
#include "lib/loader.hpp"
#include "lib/Triangle.hpp"

#define SCR_WIDTH 800
#define SCR_HEIGHT 600

int main() {
  char* vertexSourceProgram;
  char* fragmentSourceProgram;

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
    Triangle* t = new Triangle();
    t->setColor();
    t->setVertex();
    w.addObject(t);
    w.bindBuffer();
    w.run();
  } catch (std::string error) {
    glfwTerminate();
    printf("ERROR:\n%s\n", error.c_str());
    exit(1);
  } 
}
