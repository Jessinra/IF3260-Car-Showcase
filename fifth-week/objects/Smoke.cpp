#include "Smoke.hpp"
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <iostream>

const float Smoke::vertices[] =
    {
        0, 1, 0,
        1, 1, 0,
        0, 0, 0,
        1, 0, 0
    };

Smoke::Smoke(const glm::mat4 &proj, const Camera &cam, size_t maxParticles)
    : Particles(proj,
                cam,
                Shader("assets/vertexSmoke.glsl", "assets/fragmentSmoke.glsl"),
                maxParticles) {
    // Setup VAO
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

    glBindBuffer(GL_ARRAY_BUFFER, posVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * maxParticles, NULL, GL_STREAM_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // Load texture
    int width, height, nrChannels;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    // load image, create texture and generate mipmaps
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char *data = stbi_load("assets/smoke-particle.png", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
        exit(1);
    }
    stbi_image_free(data);
    // Load 1x1 texture to fix colour at a distance
    data = stbi_load("assets/smoke-1x1.png", &width, &height, &nrChannels, 0);
    if (data) {
        // 7: level of detail (210 >> 7 = 1)
        glTexImage2D(GL_TEXTURE_2D, 7, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    } else {
        std::cout << "Failed to load texture" << std::endl;
        exit(1);
    }
    stbi_image_free(data);
    shader.setInt("tex_sampler", 0);

    // Generate particles
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

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, nParticles);

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
    glDeleteTextures(1, &texture);
}
