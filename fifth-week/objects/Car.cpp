#include "Car.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <stb_image.h>

Car::Car(const glm::mat4 &proj, const Camera &cam)
    : Object(proj,
             cam,
             Shader("assets/vertexSource.glsl", "assets/fragmentSource.glsl")),
      obj("assets/jimny.obj")
{
    // Generate more VBOs
    glGenBuffers(1, &texVbo);
    glGenBuffers(1, &normVbo);

    // Construct VAO
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * obj.getConstRefPoints().size(), obj.getConstRefPoints().data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, texVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * obj.getConstRefTextures().size(), obj.getConstRefTextures().data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, normVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * obj.getConstRefNormals().size(), obj.getConstRefNormals().data(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Load texture
    int width, height, nrChannels;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char *data = stbi_load("assets/wall.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
        exit(1);
    }
    stbi_image_free(data);

    shader.setInt("texSampler", 0);
}

Car::~Car() {
    glDeleteTextures(1, &texture);
    glDeleteBuffers(1, &normVbo);
    glDeleteBuffers(1, &texVbo);
}

void Car::render(glm::vec3 lightPos) {
    // Set shader obj again
    shader.use();
    shader.setVec3("light_color", 1.0f, 1.0f, 1.0f);
    shader.setVec3("light_pos", lightPos);
    shader.setVec3("view_pos", camera.Position);
    shader.setMat4("projection", projection);
    shader.setMat4("model", glm::mat4(1.0f));
    shader.setMat4("view", camera.View);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, obj.getConstRefPoints().size());
}
