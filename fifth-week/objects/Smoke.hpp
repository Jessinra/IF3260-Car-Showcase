#ifndef SMOKE_HPP
#define SMOKE_HPP

#include <iostream>
#include <random>

#include "Particles.hpp"
#include "SmokeGenerator.hpp"

class Smoke : public Particles {
   private:
    static const float vertices[];
    float *ages;
    unsigned int texVbo, texture;
    SmokeGenerator smokeGenerator;

    void simulate();
    void generate(int nParticles);
    void reuse();

    const glm::vec3 GRAVITY = glm::vec3(0, 0.028, 0);
    const float MAX_LIFETIME = 7.0;

   public:
    Smoke(const glm::mat4 &projection, const Camera &camera,
          size_t maxParticles = 1000);

    void render(glm::vec3 lightPos);
    ~Smoke();
};

#endif
