#include "Triangle.hpp"

void Triangle::bind() {
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vboPoints);
  glGenBuffers(1, &vboColors);
  glBindVertexArray(vao);
  // Bind vertices
  glBindBuffer(GL_ARRAY_BUFFER, vboPoints);
  glBufferData(GL_ARRAY_BUFFER, 3 * vsize * sizeof(float), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  // Bind colors
  glBindBuffer(GL_ARRAY_BUFFER, vboColors);
  glBufferData(GL_ARRAY_BUFFER, 3 * csize * sizeof(float), colors, GL_STATIC_DRAW);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  // Enable vertex attrib
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  // Bind buffer
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void Triangle::draw() {
  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLES, 0, vsize);
}