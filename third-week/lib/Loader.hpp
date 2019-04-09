#ifndef LOADER_HPP
#define LOADER_HPP

#include "Mesh.hpp"

class Loader {
public:
  static void readFile(const char* file, char** buffer);
  static void fillFromObjFile(const char* path, Mesh& mesh);
};

#endif