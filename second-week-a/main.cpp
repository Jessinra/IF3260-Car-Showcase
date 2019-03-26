#include "lib/Window.hpp"
#include <string>
#include "lib/Loader.hpp"
#include "lib/MeshLoader.hpp"

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
    Mesh *m = MeshLoader::loadFile("assets/dragon.mesh");
    w.addObject(m);
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
