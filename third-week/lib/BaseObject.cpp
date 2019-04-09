#include "BaseObject.hpp"

BaseObject::BaseObject() {
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);
}

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

void BaseObject::windowTransform(glm::quat& rotationView) {
  glm::mat4 vmat = glm::make_mat4(vbuf);
  glm::mat4 rotation = glm::toMat4(rotationView);
  tvbuf = rotation * vmat;
}