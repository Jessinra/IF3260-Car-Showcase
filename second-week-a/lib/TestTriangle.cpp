#include "TestTriangle.hpp"

TestTriangle::TestTriangle() {
  vsize = 3;
  vbuf = new float[18] {
       0.0f,   0.6f, 0.0f, 1.0f, 0.0f, 0.0f,
      0.65f,  0.10f, 0.0f, 0.0f, 1.0f, 0.0f,
      0.46f, -0.74f, 0.0f, 0.0f, 0.0f, 1.0f
  };
  esize = 3;
  ebuf = new unsigned[3] { 0, 1, 2 };
}
