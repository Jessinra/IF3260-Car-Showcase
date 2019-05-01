#include "Object.hpp"

Object::Object(const glm::mat4 &projection, const Camera &camera, Shader s)
    : projection(projection), camera(camera), shader(s)
{
    // Generate buffers
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
}

Object::~Object() {
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}
