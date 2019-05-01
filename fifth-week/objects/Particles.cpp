#include "Particles.hpp"
#include <GLFW/glfw3.h>

Particles::Particles(const glm::mat4 &projection, const Camera &camera, Shader s,
                     size_t maxParticles)
    : Object(projection, camera, s),
      maxParticles(maxParticles),
      nParticles(0) {
          
    glGenBuffers(1, &posVbo);
    positions = new glm::vec3[maxParticles];
    velocities = new glm::vec3[maxParticles];
    lastTime = glfwGetTime();
}

Particles::~Particles() {
    delete[] velocities;
    delete[] positions;
    glDeleteBuffers(1, &posVbo);
}
