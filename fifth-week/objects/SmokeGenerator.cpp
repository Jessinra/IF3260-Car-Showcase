#include "SmokeGenerator.hpp"
#include <glm/gtc/random.hpp>

SmokeGenerator::SmokeGenerator() {
}

glm::vec3 SmokeGenerator::getRandomPosition() {
    return glm::vec3(-2.15, -1.25, -1.5) + glm::ballRand(1.0f) * glm::vec3(0.15, 0.25, 0.5);
}

glm::vec3 SmokeGenerator::getRandomVelocity() {
    return glm::vec3(-0.5, 0.1, 0.0) + glm::ballRand(1.0f) * glm::vec3(0.2, 0.2, 0.1);
}
