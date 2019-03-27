#ifndef MESH_HPP
#define MESH_HPP

#include "BaseObject.hpp"

class MeshLoader;

class Mesh : public BaseObject {
  friend class MeshLoader;
protected:
  unsigned dims, colElems;
  Mesh(unsigned dims, unsigned colElems, unsigned nVertices, unsigned nIndices);
public:
  void bind();
};

#endif
