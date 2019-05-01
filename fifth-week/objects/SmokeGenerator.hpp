#ifndef SMOKE_GENERATOR_HPP
#define SMOKE_GENERATOR_HPP

#include "Particles.hpp"

class SmokeGenerator {
   public:
    SmokeGenerator();
    glm::vec3 getRandomPosition();
    glm::vec3 getRandomVelocity();    
};

#endif
