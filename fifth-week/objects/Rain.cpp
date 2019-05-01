#include "Rain.hpp"
#include <GLFW/glfw3.h>

const float Rain::vertices[] =
    {
        0, 0, 0,
        -5e-3, 0.5, 5e-3 / sqrt(3),
        5e-3, 0.5, 5e-3 / sqrt(3),
        0, 0.5, -1e-2 / sqrt(3),
        0, 0, 0,
        -5e-3, 0.5, 5e-3 / sqrt(3),
    };

Rain::Rain(const glm::mat4 &proj, const Camera &cam, size_t maxParticles)
    : Particles(proj,
                cam,
                Shader("assets/vertexRain.glsl", "assets/fragmentRain.glsl"),
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

void Rain::generate(int nParticles) {
    this->nParticles = nParticles;

    std::random_device rd;                                              // obtain a random number from hardware
    std::mt19937 eng(rd());                                             // seed the generator
    std::uniform_int_distribution<> randomPosition(-10, 10);            // define the range
    std::uniform_int_distribution<> randomHeight(0, SPAWNHEIGHT * 20);  // define the range

    for (int i = 0; i < nParticles; i++) {
        int randomX = randomPosition(eng);
        int randomZ = randomPosition(eng);
        int randomY = randomHeight(eng);

        this->positions[i] = glm::vec3(randomX, randomY, randomZ);
        this->velocities[i] = STARTVELOCITY;
    }
}

void Rain::render(glm::vec3 lightPos) {
    shader.use();
    shader.setMat4("projection", projection);
    shader.setMat4("view", camera.View);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, posVbo);
    glBufferData(GL_ARRAY_BUFFER, maxParticles * sizeof(glm::vec3), NULL, GL_STREAM_DRAW);  // orphan buffer
    glBufferSubData(GL_ARRAY_BUFFER, 0, nParticles * sizeof(glm::vec3), positions);
    glVertexAttribDivisor(0, 0);  // all use the same element
    glVertexAttribDivisor(1, 1);  // each use different element

    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 6, nParticles);
    
    reuse();
    simulate();
}

void Rain::simulate() {
    double now = glfwGetTime();
    float dt = now - lastTime;

    for (size_t i = 0; i < nParticles; i++) {
        positions[i] += velocities[i] * dt + 0.5f * GRAVITY * dt * dt;
        velocities[i] += GRAVITY * dt;
    }
    lastTime = now;
}

void Rain::reuse() {
    for (int i = 0; i < nParticles; i++) {
        if (this->positions[i].y < GROUNDLEVEL) {
            this->positions[i].y = SPAWNHEIGHT;
            this->velocities[i] = STARTVELOCITY;
        }
    }
}

Rain::~Rain() {
}
