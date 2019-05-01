#ifndef RAIN_HPP
#define RAIN_HPP

#include "Particles.hpp"

class Rain : public Particles {
private:
    static const float vertices[];
    void simulate();
public:
    Rain(const glm::mat4 &projection, const Camera &camera,
         size_t maxParticles = 1000);
    void render(glm::vec3 lightPos);
    ~Rain();
};

#endif
