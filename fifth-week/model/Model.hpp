#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

using namespace std;
using namespace glm;

class Model {
protected:
    vector<glm::vec3> points;
    vector<glm::vec4> colors;
    vector<glm::vec2> textures;
    vector<glm::vec3> normals;
public:
    /* Constructor */
    Model();
    Model(const string &filename);

    void clearModel();

    int size();

    vector<vec3> getPoints();
    vector<vec3> & getRefPoints();
    const vector<vec3> & getConstRefPoints() const;

    vector<vec4> getColors();
    vector<vec4> & getRefColors();
    const vector<vec4> & getConstRefColors() const;

    vector<vec2> getTextures();
    vector<vec2> & getRefTextures();
    const vector<vec2> & getConstRefTextures() const;

    vector<vec3> getNormals();
    vector<vec3> & getRefNormals();
    const vector<vec3> & getConstRefNormals() const;

    void saveToFile(string filename);

private:
    void readFromFile(const string &filename);
    void addToArrays(const vector<float>& points, const vector<float>& colors, int size, int pointdimen, int colordimen);
};


#endif 
