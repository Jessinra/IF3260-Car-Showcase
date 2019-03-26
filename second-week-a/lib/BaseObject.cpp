#include "BaseObject.hpp"

BaseObject::~BaseObject() {
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ebo);
  delete[] vbuf;
  delete[] ebuf;
}

void BaseObject::draw() {
  glBindVertexArray(vao);
  glDrawElements(GL_TRIANGLES, esize, GL_UNSIGNED_INT, nullptr);
}
