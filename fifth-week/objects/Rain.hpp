#ifndef RAIN_HPP
#define RAIN_HPP

#include <iostream>
#include <random>

#include "Particles.hpp"

class Rain : public Particles {
   private:
    static const float vertices[];
    void simulate();
    void generate(int nParticles);
    void reuse();

    const glm::vec3 GRAVITY = glm::vec3(0, -2.8, 0);
    const float SPAWNHEIGHT = 25;
    const float GROUNDLEVEL = -2.6;
    const glm::vec3 STARTVELOCITY = glm::vec3(0, 0, 0);

   public:
    Rain(const glm::mat4 &projection, const Camera &camera,
         size_t maxParticles = 1000);

    void render(glm::vec3 lightPos);
    ~Rain();
};

#endif
