#ifndef SMOKE_GENERATOR_HPP
#define SMOKE_GENERATOR_HPP

#include <iostream>
#include <random>

#include "Particles.hpp"

class SmokeGenerator {
   private:

    std::mt19937 eng;  
    std::uniform_int_distribution<> randomX;  
    std::uniform_int_distribution<> randomY;  
    std::uniform_int_distribution<> randomZ;  

    std::uniform_int_distribution<> randomVX;  
    std::uniform_int_distribution<> randomVY;   
    std::uniform_int_distribution<> randomVZ;   
   public:
    SmokeGenerator();
    glm::vec3 getRandomPosition();
    glm::vec3 getRandomVelocity();
    
};

#endif
