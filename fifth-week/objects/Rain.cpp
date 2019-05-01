#include "Rain.hpp"
#include <GLFW/glfw3.h>

const float Rain::vertices[] =
    {
     0, 0, 0,
     -0.25, 0.5, 0,
     0.25, 0.5, 0
    };

Rain::Rain(const glm::mat4 &proj, const Camera &cam, size_t maxParticles)
    : Particles(proj,
                cam,
                Shader("assets/vertexRain.glsl", "assets/fragmentRain.glsl"),
                maxParticles)
{
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, posVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * maxParticles, NULL, GL_STREAM_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    nParticles = 1;
    velocities[0] = glm::vec3(0, 1, 0);
    positions[0] = glm::vec3(0, 3.0, 0);
}

void Rain::render(glm::vec3 lightPos) {
    shader.use();
    shader.setMat4("projection", projection);
    shader.setMat4("view", camera.View);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, posVbo);
    glBufferData(GL_ARRAY_BUFFER, maxParticles * sizeof(glm::vec3), NULL, GL_STREAM_DRAW); // orphan buffer
    glBufferSubData(GL_ARRAY_BUFFER, 0, 1 * sizeof(glm::vec3), positions);
    glVertexAttribDivisor(0, 0); // all use the same element
    glVertexAttribDivisor(1, 1); // each use different element
    glDrawArraysInstanced(GL_TRIANGLES, 0, 3, 1);
    simulate();
    //glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Rain::simulate() {
    double now = glfwGetTime();
    float dt = now - lastTime;
    glm::vec3 accel(0, -9.8, 0);
    for (size_t i = 0; i < nParticles; i++) {
        positions[i] += velocities[i] * dt + 0.5f * accel * dt * dt;
        velocities[i] += accel * dt;
    }
    lastTime = now;
}

Rain::~Rain() {
}
