#ifndef PARTICLES_HPP
#define PARTICLES_HPP

#include "Object.hpp"

class Particles : public Object {
protected:
    const size_t maxParticles;
    glm::vec3 *positions;
    glm::vec3 *velocities;
    unsigned int posVbo;
    size_t nParticles;
    double lastTime;
    virtual void simulate() = 0;
public:
    Particles(const glm::mat4 &projection, const Camera &camera, Shader,
              size_t maxParticles = 1000);
    virtual void render(glm::vec3 lightPos) = 0;
    virtual ~Particles();
};

#endif
