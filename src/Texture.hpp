#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "shaders/shader.hpp"

class Texture {
public:
    Texture() = default;

    Texture(const std::string & filename, float shininess = 128.0f);

    void loadTexture(const std::string & filename);

    std::string getFilename() const;

    void setShininess(float shininess);

    void bindTextureDiffuse(const Shader & shader, unsigned int id, int index) const;
    void bindTextureSpecular(const Shader & shader, unsigned int id, int index) const;
    void bindTextureEmbient(const Shader & shader, unsigned int id, int index) const;


    bool operator==(const std::string & filename) const;
    bool isLoaded()                               const;

    ~Texture();
private:
    std::string _filename = "";


    unsigned int _texture = 0;

    float _shininess = 128.0f;
};

#endif