#include "Texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

Texture::Texture(const std::string & filename, float shininess)
: _filename(filename), _shininess(shininess), _texture(0) {
    loadTexture(_filename);
}

void Texture::bindTextureDiffuse(const Shader & shader, unsigned int id, int index) const {
    if (_texture == 0) {
        std::cout << "Error binding texture: the texture was not created\n";
        return;
    }

    shader.uniform1i("material._diffuse[" + std::to_string(index) + "]",   id);

    glActiveTexture(GL_TEXTURE0 + id);
    glBindTexture(GL_TEXTURE_2D, _texture);
}
void Texture::bindTextureSpecular(const Shader & shader, unsigned int id, int index) const {
    if (_texture == 0) {
        std::cout << "Error binding texture: the texture was not created\n";
        return;
    }

    shader.uniform1i("material._specular[" + std::to_string(index) + "]",   id);
    shader.uniform1f("material._shininess[" + std::to_string(index) + "]", _shininess);

    glActiveTexture(GL_TEXTURE0 + id);
    glBindTexture(GL_TEXTURE_2D, _texture);
}
void Texture::bindTextureEmbient(const Shader & shader, unsigned int id, int index) const {
    shader.uniform1i("material._embient[" + std::to_string(index) + "]",   id);

    glActiveTexture(GL_TEXTURE0 + id);
    glBindTexture(GL_TEXTURE_2D, _texture);
}




void Texture::loadTexture(const std::string & filename) {
    stbi_set_flip_vertically_on_load(true); 
    _filename = filename;

    glDeleteTextures(1, &_texture);
    
    if (_filename.empty()) return;
    if (_filename.find(".") == std::string::npos) {
        _texture = 0;
        std::cout << "Error loading texture: File extension not specified (" << _filename << ")\n";
        return;
    }

    glGenTextures(1, &_texture);
    glBindTexture(GL_TEXTURE_2D, _texture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    int width, height, nrChanels;
    unsigned char * data = stbi_load(_filename.c_str(), &width, &height, &nrChanels, 0);

    std::string expansion = _filename.substr(_filename.find_last_of('.'));

    if (data)
    {         
        if      (expansion == ".jpg") glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        else if (expansion == ".png") {
            if      (nrChanels == 4) glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            else if (nrChanels == 3) glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            else {
                std::cout << "Error loading texture: Unsupported number of channels (" << nrChanels << ")\n";
                _texture = 0;
            }
        }
        else {
            std::cout << "Error loading texture: this file format is not supported (" << expansion << ")\n";
            _texture = 0;
        }
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture: " << _filename << '\n';
        _texture = 0;
    }
    
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

bool Texture::operator==(const std::string & filename) const {  return _filename == filename;   }
bool Texture::isLoaded()                               const {  return _texture;                }

std::string Texture::getFilename() const {  return _filename;   }

void Texture::setShininess(float shininess) {    _shininess = shininess;    }

Texture::~Texture() {
    //glDeleteTextures(1, &_texture);
}