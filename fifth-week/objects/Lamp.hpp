#ifndef LAMP_HPP
#define LAMP_HPP

#include "Object.hpp"

class Lamp : public Object {
private:
    static const float vertices[];
public:
    Lamp(const glm::mat4 &projection, const Camera &camera);
    void render(glm::vec3 lightPos);
    ~Lamp();
};

#endif
