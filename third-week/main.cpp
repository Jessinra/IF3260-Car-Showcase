#include <string>
#include "lib/Window.hpp"
#include "lib/Loader.hpp"
#include "lib/Shader.hpp"
#include "lib/Camera.hpp"

#define SCR_WIDTH 1200
#define SCR_HEIGHT 1000

glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
glm::mat4 view = glm::mat4(1.0);
glm::mat4 model = glm::mat4(1.0);
Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));

int main() {
  try {
    Window w(SCR_WIDTH, SCR_HEIGHT, "Jeep");
    w.initializeGLFW();
    w.createWindow();
    w.initializeGLAD();

    Shader s("assets/vectorSource.glsl", "assets/fragmentSource.glsl");
    s.use();

    Mesh* m = new Mesh();
    Loader::fillFromObjFile("assets/jimny.obj", *m);
    w.addObject(m);
    w.bindBuffer();
    w.bindTexture(s);
    w.run(s, camera, view);

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
