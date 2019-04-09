#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include "../modules/glad/glad.h" 
#include <glm/glm.hpp>

using namespace std;

class Shader {
public:
    unsigned int StackedShader;

    Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);

    // activate the shader
    void use();

    // utility uniform functions
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setVec2(const std::string &name, const glm::vec2 &value) const;
    void setVec2(const std::string &name, float x, float y) const;
    void setVec3(const std::string &name, const glm::vec3 &value) const;
    void setVec3(const std::string &name, float x, float y, float z) const;
    void setVec4(const std::string &name, const glm::vec4 &value) const;
    void setVec4(const std::string &name, float x, float y, float z, float w);
    void setMat2(const std::string &name, const glm::mat2 &mat) const;
    void setMat3(const std::string &name, const glm::mat3 &mat) const;
    void setMat4(const std::string &name, const glm::mat4 &mat) const;

private:

    unsigned int initializeVertexShader(const char* vertexPath);
    unsigned int initializeFragmentShader(const char* fragmentPath);
    unsigned int initializeGeometryShader(const char* geometryPath);

    std::string loadShader(const char* shaderPath);
    unsigned int compileToVertexShader(std::string shaderCode);
    unsigned int compileToFragmentShader(std::string shaderCode);
    unsigned int compileToGeometryShader(std::string shaderCode);

    void attachThenDeleteShadder(unsigned int shadder);

    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void checkCompileErrors(GLuint shader, std::string type);
};

#endif
