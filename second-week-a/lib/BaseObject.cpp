#include "BaseObject.hpp"

void BaseObject::setColor() {
  // Stub
  csize = 3;
  colors = new float[9] {
    1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f
  };
}

void BaseObject::setVertex() {
  // Stub
  vsize = 3;
  vertices = new float[9] {
    0.0f, 0.6f, 0.0f,     
    0.65f, 0.10f, 0.0f,   
    0.46f, -0.74f, 0.0f
  };
}

BaseObject::~BaseObject() {
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vboPoints);
  glDeleteBuffers(1, &vboColors);
  delete[] colors;
  delete[] vertices;
}