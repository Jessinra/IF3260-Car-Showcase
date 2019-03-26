#ifndef POLYGON_HPP
#define POLYGON_HPP

#include "BaseObject.hpp"

class Polygon: public BaseObject {
protected:
  float *indices;
public:
  void bind();
  void draw();
};

#endif