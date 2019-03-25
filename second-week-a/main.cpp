#include "lib/Window.h"
#include <string>

#define SCR_WIDTH 800
#define SCR_HEIGHT 600

int main() {
  Window w(SCR_WIDTH, SCR_HEIGHT, "Duragon");
  try {
    w.initializeGLFW();
    w.createWindow();
    w.initializeGLFW();
    w.initializeGLAD();
  } catch (std::string error) {
    printf("ERROR:\n%s\n", error.c_str());
    exit(1);
  } 
}
