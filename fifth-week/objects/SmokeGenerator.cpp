#include "SmokeGenerator.hpp"
#include <glm/gtc/random.hpp>

SmokeGenerator::SmokeGenerator() {
}

glm::vec3 SmokeGenerator::getRandomPosition() {
    return glm::vec3(-2.15, -1.25, -1.5) + glm::ballRand(0.25f);
}

glm::vec3 SmokeGenerator::getRandomVelocity() {
    return glm::vec3(-0.3, 0.1, 0.0) + glm::ballRand(0.2f);
}
