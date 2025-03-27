#include "Model.hpp"

#include <algorithm>

Model::Model(const std::vector<std::shared_ptr<Mesh>> & meshes, const std::string & modelName, 
            const std::vector<std::shared_ptr<Texture>> & texturesDiffuse,     
                const std::vector<std::shared_ptr<Texture>> & texturesSpecular,
                    const std::vector<std::shared_ptr<Texture>> & texturesEmbient)
: Object(modelName), _meshes(meshes), _texturesDiffuse(texturesDiffuse), _texturesSpecular(texturesSpecular), _texturesEmbient(texturesEmbient) {}

void Model::ApplyTextureDiffuse(const std::string & textureDiffuseName) {
    for (int i = 0; i < _texturesDiffuse.size(); ++i) {
        if (*_texturesDiffuse[i].get() == textureDiffuseName) {
            std::cout << "Error loading texture " << textureDiffuseName << " : texture is already loading\n";
            return;
        }
    }

    std::shared_ptr<Texture> texture = std::make_shared<Texture>(textureDiffuseName);

    if (texture->isLoaded()) {
        _texturesDiffuse.push_back(texture);

        for (auto mesh : _meshes) {
            mesh->loadTextureDiffuse(_texturesDiffuse[_texturesDiffuse.size() - 1].get());
        }
    }
}

void Model::ApplyTextureSpecular(const std::string & textureSpecularName) {
    for (int i = 0; i < _texturesSpecular.size(); ++i) {
        if (*_texturesSpecular[i].get() == textureSpecularName) {
            std::cout << "Error loading texture " << textureSpecularName << " : texture is already loading\n";
            return;
        }
    }

    std::shared_ptr<Texture> texture = std::make_shared<Texture>(textureSpecularName);

    if (texture->isLoaded()) {
        _texturesSpecular.push_back(texture);

        for (auto mesh : _meshes) {
            mesh->loadTextureSpecular(_texturesSpecular[_texturesSpecular.size() - 1].get());
        }
    }
}

void Model::ApplyTextureAmbient(const std::string & textureEmbientName) {
    for (int i = 0; i < _texturesEmbient.size(); ++i) {
        if (*_texturesEmbient[i].get() == textureEmbientName) {
            std::cout << "Error loading texture " << textureEmbientName << " : texture is already loading\n";
            return;
        }
    }

    std::shared_ptr<Texture> texture = std::make_shared<Texture>(textureEmbientName);

    if (texture->isLoaded()) {
        _texturesEmbient.push_back(texture);

        for (auto mesh : _meshes) {
            mesh->loadTextureEmbient(_texturesEmbient[_texturesEmbient.size() - 1].get());
        }
    }
}

const std::vector<std::shared_ptr<Mesh>> &    Model::getMeshes()           const {   return _meshes;           }
const std::vector<std::shared_ptr<Texture>> & Model::getTexturesDiffuse()  const {   return _texturesDiffuse;  }
const std::vector<std::shared_ptr<Texture>> & Model::getTexturesSpecular() const {   return _texturesSpecular; }
const std::vector<std::shared_ptr<Texture>> & Model::getTexturesEmbient()  const {   return _texturesEmbient;  }


Model Model::getCube(const glm::vec4 & color, float size, const std::string & objectName) {
    return {{Mesh::getCube(color, size, "cube")}, objectName};
}