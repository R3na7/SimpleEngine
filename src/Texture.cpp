#include "Texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

Texture::Texture(const std::string &filename, float shininess)
    : _filename(filename), _shininess(shininess), _texture(0), _isHDR(false) {
    loadTexture(_filename);
}

void Texture::bindTextureDiffuse(const Shader &shader, unsigned int id, int index) const {
    if (_texture == 0) {
        std::cerr << "Error binding texture: the texture was not created\n";
        return;
    }
    shader.uniform1i("material._diffuse[" + std::to_string(index) + "]", id);
    glActiveTexture(GL_TEXTURE0 + id);
    glBindTexture(GL_TEXTURE_2D, _texture);
}

void Texture::bindTextureSpecular(const Shader &shader, unsigned int id, int index) const {
    if (_texture == 0) {
        std::cerr << "Error binding texture: the texture was not created\n";
        return;
    }
    shader.uniform1i("material._specular[" + std::to_string(index) + "]", id);
    shader.uniform1f("material._shininess[" + std::to_string(index) + "]", _shininess);
    glActiveTexture(GL_TEXTURE0 + id);
    glBindTexture(GL_TEXTURE_2D, _texture);
}

void Texture::bindTextureAmbient(const Shader &shader, unsigned int id, int index) const {
    if (_texture == 0) {
        std::cerr << "Error binding texture: the texture was not created\n";
        return;
    }
    shader.uniform1i("material._ambient[" + std::to_string(index) + "]", id);
    glActiveTexture(GL_TEXTURE0 + id);
    glBindTexture(GL_TEXTURE_2D, _texture);
}

void Texture::loadTexture(const std::string &filename) {
    stbi_set_flip_vertically_on_load(true);
    _filename = filename;

    if (_texture != 0) {
        glDeleteTextures(1, &_texture);
        _texture = 0;
    }

    if (_filename.empty()) {
        std::cerr << "Error: Texture filename is empty\n";
        return;
    }

    if (_filename.find(".") == std::string::npos) {
        std::cerr << "Error: File extension not specified (" << _filename << ")\n";
        return;
    }

    _isHDR = (_filename.substr(_filename.find_last_of('.')) == ".hdr");

    int width, height, nrChannels;
    void *data = nullptr;

    if (_isHDR) {
        data = stbi_loadf(_filename.c_str(), &width, &height, &nrChannels, 0);
    } else {
        data = stbi_load(_filename.c_str(), &width, &height, &nrChannels, 0);
    }

    if (!data) {
        std::cerr << "Failed to load texture: " << _filename << "\n";
        return;
    }

    if (width <= 0 || height <= 0) {
        std::cerr << "Error: Invalid texture dimensions (" << width << "x" << height << ")\n";
        stbi_image_free(data);
        return;
    }

    GLenum internalFormat, format;
    switch (nrChannels) {
        case 1:
            internalFormat = _isHDR ? GL_R16F : GL_RED;
            format = GL_RED;
            break;
        case 2:
            internalFormat = _isHDR ? GL_RG16F : GL_RG;
            format = GL_RG;
            break;
        case 3:
            internalFormat = _isHDR ? GL_RGB16F : GL_RGB;
            format = GL_RGB;
            break;
        case 4:
            internalFormat = _isHDR ? GL_RGBA16F : GL_RGBA;
            format = GL_RGBA;
            break;
        default:
            std::cerr << "Error: Unsupported number of channels (" << nrChannels << ")\n";
            stbi_image_free(data);
            return;
    }

    glGenTextures(1, &_texture);
    glBindTexture(GL_TEXTURE_2D, _texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (_isHDR) {
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_FLOAT, data);
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    }
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

bool Texture::operator==(const std::string &filename) const { return _filename == filename; }
bool Texture::isLoaded() const { return _texture != 0; }
bool Texture::isHDR() const { return _isHDR; }

std::string Texture::getFilename() const { return _filename; }
void Texture::setShininess(float shininess) { _shininess = shininess; }

Texture::~Texture() {
    if (_texture != 0) {
        glDeleteTextures(1, &_texture);
    }
}