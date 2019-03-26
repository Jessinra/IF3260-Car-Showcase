#include "Triangle.hpp"

void Triangle::bind() {
  glBindVertexArray(vao);
  // Bind vertices
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, 6 * vsize * sizeof(float), vbuf,
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  // Bind colors
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void*)(3 * sizeof(float)));
  // Enable vertex attrib
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  // Bind indices
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, esize * sizeof(unsigned), ebuf,
               GL_STATIC_DRAW);
  /* Is this even needed?
  // Unind buffer
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  */
}
