#include "Shader.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

Shader::Shader(const char *vertexPath, const char *fragmentPath, const char *geometryPath) {
    StackedShader = glCreateProgram();
    try {
        if (vertexPath != nullptr) {
            initializeVertexShader(vertexPath);
        }

        if (fragmentPath != nullptr) {
            initializeFragmentShader(fragmentPath);
        }

        if (geometryPath != nullptr) {
            initializeGeometryShader(geometryPath);
        }

    } catch (std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    // shader Program
    glBindAttribLocation(StackedShader, 0, "vertex_position");
    glBindAttribLocation(StackedShader, 1, "vertex_colour");
    glLinkProgram(StackedShader);
    checkCompileErrors(StackedShader, "PROGRAM");
}

unsigned int Shader::initializeVertexShader(const char *vertexPath) {
    std::string shaderCode;
    unsigned int shader;

    shaderCode = loadShader(vertexPath);
    shader = compileToVertexShader(shaderCode);

    attachThenDeleteShadder(shader);
}

unsigned int Shader::initializeFragmentShader(const char *fragmentPath) {
    std::string fragmentShaderCode;
    unsigned int fragmentShader;

    fragmentShaderCode = loadShader(fragmentPath);
    fragmentShader = compileToFragmentShader(fragmentShaderCode);

    attachThenDeleteShadder(fragmentShader);
}

unsigned int Shader::initializeGeometryShader(const char *geometryPath) {
    std::string geometryShaderCode;
    unsigned int geometryShader;

    geometryShaderCode = loadShader(geometryPath);
    geometryShader = compileToGeometryShader(geometryShaderCode);

    attachThenDeleteShadder(geometryShader);
}

std::string Shader::loadShader(const char *shaderPath) {
    std::ifstream shaderFile;
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    std::stringstream shaderStream;

    shaderFile.open(shaderPath);
    shaderStream << shaderFile.rdbuf();
    shaderFile.close();

    return shaderStream.str();
}

unsigned int Shader::compileToVertexShader(std::string shaderCode) {
    const char *cShaderCode = shaderCode.c_str();
    unsigned int shader;

    shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shader, 1, &cShaderCode, NULL);
    glCompileShader(shader);
    checkCompileErrors(shader, "VERTEX");

    return shader;
}

unsigned int Shader::compileToFragmentShader(std::string shaderCode) {
    const char *cShaderCode = shaderCode.c_str();
    unsigned int shader;

    shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shader, 1, &cShaderCode, NULL);
    glCompileShader(shader);
    checkCompileErrors(shader, "FRAGMENT");

    return shader;
}

unsigned int Shader::compileToGeometryShader(std::string shaderCode) {
    const char *cShaderCode = shaderCode.c_str();
    unsigned int shader;

    shader = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(shader, 1, &cShaderCode, NULL);
    glCompileShader(shader);
    checkCompileErrors(shader, "GEOMETRY");

    return shader;
}

void Shader::attachThenDeleteShadder(unsigned int shadder) {
    glAttachShader(StackedShader, shadder);
    glDeleteShader(shadder);
}

void Shader::use() {
    glUseProgram(StackedShader);
}

void Shader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(StackedShader, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(StackedShader, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(StackedShader, name.c_str()), value);
}

void Shader::setVec2(const std::string &name, const glm::vec2 &value) const {
    glUniform2fv(glGetUniformLocation(StackedShader, name.c_str()), 1, &value[0]);
}

void Shader::setVec2(const std::string &name, float x, float y) const {
    glUniform2f(glGetUniformLocation(StackedShader, name.c_str()), x, y);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) const {
    glUniform3fv(glGetUniformLocation(StackedShader, name.c_str()), 1, &value[0]);
}

void Shader::setVec3(const std::string &name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(StackedShader, name.c_str()), x, y, z);
}

void Shader::setVec4(const std::string &name, const glm::vec4 &value) const {
    glUniform4fv(glGetUniformLocation(StackedShader, name.c_str()), 1, &value[0]);
}

void Shader::setVec4(const std::string &name, float x, float y, float z, float w) {
    glUniform4f(glGetUniformLocation(StackedShader, name.c_str()), x, y, z, w);
}

void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const {
    glUniformMatrix2fv(glGetUniformLocation(StackedShader, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const {
    glUniformMatrix3fv(glGetUniformLocation(StackedShader, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(StackedShader, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::checkCompileErrors(GLuint shader, std::string type) {
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                      << infoLog
                      << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                      << infoLog
                      << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}