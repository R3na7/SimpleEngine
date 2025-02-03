#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>
#include <fstream>


class Shader {
private:
    unsigned int ID;

public:
    Shader (const char * vertexPath, const char * fragmentPath);

    void use();

    unsigned int getId() const;

    void uniformMatrix4(const std::string & name, const glm::mat4 & matrix) const;

    void uniform1i(const std::string & name, int value) const;

    void uniform4f(const std::string & name, const glm::vec4 & vector) const;

    void uniform3f(const std::string & name, const glm::vec3 & vector) const;

    void uniform1f(const std::string & name, float value) const;
    ~Shader();
    
};


#endif