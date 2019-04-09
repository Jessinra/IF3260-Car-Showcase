#ifndef BASE_OBJECT_HPP
#define BASE_OBJECT_HPP

#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

class BaseObject {
protected:
  int vsize, esize;
  unsigned int vao, vbo, ebo;
  float *vbuf;
  glm::mat4 tvbuf; // transformed v buffer;
  unsigned int *ebuf;
  BaseObject();
public:
  virtual void bind()=0;
  virtual void draw();
  virtual void windowTransform(glm::quat& rotationView);
  virtual ~BaseObject();
};

#endif
