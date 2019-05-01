#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "../modules/glad/glad.h"
#include <glm/glm.hpp>
#include "../lib/Shader.hpp"
#include "../lib/Camera.hpp"

class Object {
protected:
    unsigned int vao, vbo;
    Shader shader;
    const glm::mat4 &projection;
    const Camera &camera;
public:
    Object(const glm::mat4 &projection, const Camera &camera, Shader);
    virtual void render(glm::vec3 lightPos) = 0;
    virtual ~Object();
};

#endif
