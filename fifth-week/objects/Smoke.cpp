#include "Smoke.hpp"
#include <GLFW/glfw3.h>

const float Smoke::vertices[] =
    {
        0, 0, 0,
        -0.02, 0.02, 0,
        0.02, 0.02, 0};

Smoke::Smoke(const glm::mat4 &proj, const Camera &cam, size_t maxParticles)
    : Particles(proj,
                cam,
                Shader("assets/vertexSmoke.glsl", "assets/fragmentSmoke.glsl"),
                maxParticles) {
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

    glBindBuffer(GL_ARRAY_BUFFER, posVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * maxParticles, NULL, GL_STREAM_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    this->generate(maxParticles);
}

void Smoke::generate(int nParticles) {
    this->nParticles = nParticles;

    for (int i = 0; i < nParticles; i++) {
        this->positions[i] = this->smokeGenerator.getRandomPosition();
        this->velocities[i] = this->smokeGenerator.getRandomVelocity();
    }
}

void Smoke::render(glm::vec3 lightPos) {
    shader.use();
    shader.setMat4("projection", projection);
    shader.setMat4("view", camera.View);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, posVbo);
    glBufferData(GL_ARRAY_BUFFER, maxParticles * sizeof(glm::vec3), NULL, GL_STREAM_DRAW);  // orphan buffer
    glBufferSubData(GL_ARRAY_BUFFER, 0, nParticles * sizeof(glm::vec3), positions);
    glVertexAttribDivisor(0, 0);  // all use the same element
    glVertexAttribDivisor(1, 1);  // each use different element

    glDrawArraysInstanced(GL_TRIANGLES, 0, 3, nParticles);

    reuse();
    simulate();
}

void Smoke::simulate() {
    double now = glfwGetTime();
    float dt = now - lastTime;

    for (size_t i = 0; i < nParticles; i++) {
        positions[i] += velocities[i] * dt + 0.5f * GRAVITY * dt * dt;
        velocities[i] += GRAVITY * dt;
    }
    lastTime = now;
}

void Smoke::reuse() {

    for (int i = 0; i < nParticles; i++) {
        
        if (this->positions[i].y > DISPERSEHEIGHT || this->positions[i].x < BACKWALL) {
            this->positions[i] = this->smokeGenerator.getRandomPosition();
            this->velocities[i] = this->smokeGenerator.getRandomVelocity();
        }
    }
}

Smoke::~Smoke() {
}
