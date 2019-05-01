#include "SmokeGenerator.hpp"
#include <GLFW/glfw3.h>

SmokeGenerator::SmokeGenerator() {
    std::random_device rd;           // obtain a random number from hardware
    this->eng = std::mt19937(rd());  // seed the generator

    this->randomX = std::uniform_int_distribution<>(-230, -200);  // define the range
    this->randomY = std::uniform_int_distribution<>(-150, -100);  // define the range
    this->randomZ = std::uniform_int_distribution<>(-200, -100);  // define the range

    this->randomVX = std::uniform_int_distribution<>(-500, -100);  // define the range
    this->randomVY = std::uniform_int_distribution<>(-100, 300);   // define the range
    this->randomVZ = std::uniform_int_distribution<>(-100, 100);   // define the range
}

glm::vec3 SmokeGenerator::getRandomPosition() {
    float X = randomX(eng) / 100.0;
    float Y = randomY(eng) / 100.0;
    float Z = randomZ(eng) / 100.0;

    return glm::vec3(X, Y, Z);
}

glm::vec3 SmokeGenerator::getRandomVelocity() {
    float VX = randomVX(eng) / 1000.0;
    float VY = randomVY(eng) / 1000.0;
    float VZ = randomVZ(eng) / 1000.0;

    return glm::vec3(VX, VY, VZ);
}
