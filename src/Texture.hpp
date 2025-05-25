#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Shaders/Shader.hpp"

class Texture {
public:
    Texture() = default;

    Texture(const std::string & filename, float shininess = 128.0f);

    Texture(const Texture & other);

    Texture(Texture&& other);

    Texture& operator=(const Texture & other);
    Texture& operator=(Texture&& other);

    void loadTexture(const std::string & filename);

    std::string getFilename() const;
    bool isHDR() const;

    void setShininess(float shininess);

    void bindTextureDiffuse(const Shader & shader, unsigned int id, int index) const;
    void bindTextureSpecular(const Shader & shader, unsigned int id, int index) const;
    void bindTextureAmbient(const Shader & shader, unsigned int id, int index) const;


    bool operator==(const std::string & filename) const;
    bool isLoaded()                               const;
    bool isOwner()                                const;

    ~Texture();
private:
    std::string _filename = "";
    bool _isHDR;
    
    bool _isOwner;
    unsigned int _texture = 0;

    float _shininess = 128.0f;
};

#endif
