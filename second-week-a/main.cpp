#include "lib/Window.hpp"
#include <string>
#include "lib/loader.hpp"
#include "lib/TestTriangle.hpp"

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
    Triangle* t = new TestTriangle();
    w.addObject(t);
    w.bindBuffer();
    w.run();
  } catch (const std::string &error) {
    glfwTerminate();
    printf("ERROR:\n%s\n", error.c_str());
    exit(1);
  } catch (const char *error) {
    glfwTerminate();
    printf("ERROR:\n%s\n", error);
    exit(1);
  }
}
