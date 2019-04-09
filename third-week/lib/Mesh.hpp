#ifndef MESH_HPP
#define MESH_HPP

#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class Mesh {
protected:
  unsigned int vao, point_vbo, tex_vbo, ebo;
public:
  // Properties
  std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
  std::vector<glm::vec3> vertices;
  std::vector<glm::vec2> uvs;
  std::vector<glm::vec3> normals;
  // Functions
  Mesh();
  void bind();
  void draw();
  ~Mesh();
};

#endif
