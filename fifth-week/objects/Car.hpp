#ifndef CAR_HPP
#define CAR_HPP

#include "Object.hpp"
#include "../model/Model.hpp"

class Car : public Object {
private:
    Model obj;
    unsigned int texVbo, normVbo, texture;
public:
    Car(const glm::mat4 &projection, const Camera &camera);
    void render(glm::vec3 lightPos);
    ~Car();
};

#endif
