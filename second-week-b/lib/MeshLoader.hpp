#ifndef MESHLOADER_HPP
#define MESHLOADER_HPP

#include <istream>
#include "Mesh.hpp"

class MeshLoader {
public:
  static Mesh *parse(std::istream &input);
  static Mesh *loadFile(const char *path);
};

#endif
