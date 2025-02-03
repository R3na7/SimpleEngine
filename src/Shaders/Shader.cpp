#ifndef SHADER_CPP
#define SHADER_CPP

#include "shader.hpp"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{   
    std::ifstream ShaderFile;

    std::stringstream vertexShaderStream;
    std::stringstream fragmentShaderStream;

    ShaderFile.open(vertexPath);
    if (!ShaderFile) {
        std::cout << "error: the file " << vertexPath << " could not be opened\n";
        std::terminate();
    }

    std::string vertexShaderString;
    std::string fragmentShaderString;

    vertexShaderStream << ShaderFile.rdbuf();
    vertexShaderString = vertexShaderStream.str();

    ShaderFile.close();

    ShaderFile.open(fragmentPath);
    if (!ShaderFile) {
        std::cout << "error: the file " << fragmentPath << " could not be opened\n";
        std::terminate();
    }

    fragmentShaderStream << ShaderFile.rdbuf();
    fragmentShaderString = fragmentShaderStream.str();

    ShaderFile.close();

    const char *vertexShaderCode = vertexShaderString.c_str();
    const char *fragmentShaderCode = fragmentShaderString.c_str();

    unsigned int vertex, fragment;
    int succes;
    char infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexShaderCode, NULL);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &succes);
    if (!succes) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "error: vertex shader compilation failed\n" << infoLog << '\n';
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentShaderCode, NULL);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &succes);
    if(!succes) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "error: fragment shader compilation failed\n" << infoLog << '\n';
    }

    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    glGetProgramiv(ID, GL_LINK_STATUS, &succes);
    if(!succes) {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "error: shader program linking failed\n" << infoLog << '\n';
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}
void Shader::use() {
    glUseProgram(ID);
}

void Shader::uniformMatrix4(const std::string & name, const glm::mat4 & matrix) const {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::uniform4f(const std::string & name, const glm::vec4 & vector) const {
    glUniform4f(glGetUniformLocation(ID, name.c_str()), vector.x, vector.y, vector.z, vector.w);
}

void Shader::uniform3f(const std::string & name, const glm::vec3 & vector) const {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), vector.x, vector.y, vector.z);
}

void Shader::uniform1f(const std::string & name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::uniform1i(const std::string & name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

unsigned int Shader::getId() const {    return ID;  }

Shader::~Shader() {
    glDeleteShader(ID);
}

#endif