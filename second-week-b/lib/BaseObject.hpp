#ifndef BASE_OBJECT_HPP
#define BASE_OBJECT_HPP

#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>

class BaseObject {
protected:
  int vsize, esize;
  unsigned int vao, vbo, ebo;
  float *vbuf;
  unsigned int *ebuf;
  BaseObject();
public:
  virtual void bind()=0;
  virtual void draw();
  virtual ~BaseObject();
};

#endif
