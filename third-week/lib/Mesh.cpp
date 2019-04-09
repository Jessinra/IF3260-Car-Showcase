#include "Mesh.hpp"
#include <cstdio>

Mesh::Mesh() {
}

Mesh::~Mesh() {
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &point_vbo);
  glDeleteBuffers(1, &tex_vbo);
  glDeleteBuffers(1, &ebo);
}

void Mesh::bind() {
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &point_vbo);
  glGenBuffers(1, &tex_vbo);
  glGenBuffers(1, &ebo);

  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, point_vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), 
               vertices.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  
  glBindBuffer(GL_ARRAY_BUFFER, tex_vbo);
  glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), 
               uvs.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void Mesh::draw() {
  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}
