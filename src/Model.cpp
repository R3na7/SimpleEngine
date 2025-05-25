#include "Model.hpp"

#include <algorithm>

Model::Model(const std::vector<Mesh> & meshes, const std::string & modelName, 
            const std::vector<Texture> & texturesDiffuse,     
                const std::vector<Texture> & texturesSpecular,
                    const std::vector<Texture> & texturesEmbient)
: Object(modelName), _meshes(meshes), 
_texturesDiffuse(std::move(texturesDiffuse)), 
_texturesSpecular(std::move(texturesSpecular)), 
_texturesEmbient(std::move(texturesEmbient)) {}

void Model::ApplyTextureDiffuse(const std::string & textureDiffuseName) {
    for (int i = 0; i < _texturesDiffuse.size(); ++i) {
        if (_texturesDiffuse[i] == textureDiffuseName) {
            std::cout << "Error loading texture " << textureDiffuseName << " : texture is already loading\n";
            return;
        }
    }

    Texture texture = textureDiffuseName;

    
    if (texture.isLoaded()) {
        _texturesDiffuse.push_back(std::move(texture));

        for (auto & mesh : _meshes) {
            mesh.loadTextureDiffuse(_texturesDiffuse[_texturesDiffuse.size() - 1]);
        }
    } else {
        std::cout << "Error from ApplyTextureDiffuse: " << texture.getFilename() << " is not loaded\n";
    }
}

void Model::ApplyTextureSpecular(const std::string & textureSpecularName) {
    for (int i = 0; i < _texturesSpecular.size(); ++i) {
        if (_texturesSpecular[i] == textureSpecularName) {
            std::cout << "Error loading texture " << textureSpecularName << " : texture is already loading\n";
            return;
        }
    }

    Texture texture = textureSpecularName;

    if (texture.isLoaded()) {
        _texturesSpecular.push_back(std::move(texture));

        for (auto & mesh : _meshes) {
            mesh.loadTextureSpecular(_texturesSpecular[_texturesSpecular.size() - 1]);
        }
    }
}

void Model::ApplyTextureAmbient(const std::string & textureEmbientName) {
    for (int i = 0; i < _texturesEmbient.size(); ++i) {
        if (_texturesEmbient[i] == textureEmbientName) {
            std::cout << "Error loading texture " << textureEmbientName << " : texture is already loading\n";
            return;
        }
    }

    Texture texture = textureEmbientName;

    if (texture.isLoaded()) {
        _texturesEmbient.push_back(std::move(texture));

        for (auto & mesh : _meshes) {
            mesh.loadTextureEmbient(_texturesEmbient[_texturesEmbient.size() - 1]);
        }
    }
}

const std::vector<Mesh>    & Model::getMeshes()           const {   return _meshes;           }
const std::vector<Texture> & Model::getTexturesDiffuse()  const {   return _texturesDiffuse;  }
const std::vector<Texture> & Model::getTexturesSpecular() const {   return _texturesSpecular; }
const std::vector<Texture> & Model::getTexturesEmbient()  const {   return _texturesEmbient;  }


Model Model::getCube(const glm::vec4 & color, float size, const std::string & objectName) {
    return {{Mesh::getCube(color, size, "cube")}, objectName};
}