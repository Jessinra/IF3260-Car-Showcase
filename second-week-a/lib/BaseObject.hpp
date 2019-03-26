#ifndef BASE_OBJECT_HPP
#define BASE_OBJECT_HPP

#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>

class BaseObject {
protected:
  int vsize, csize;
  unsigned int vao, vboPoints, vboColors;
  float *vertices;
  float *colors;
public:
  void setVertex();
  void setColor();
  virtual void bind()=0;
  virtual void draw()=0;
  virtual ~BaseObject();
};

#endif