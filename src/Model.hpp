#ifndef MODEL_HPP
#define MODEL_HPP

#include <memory>
#include "Mesh.hpp"

class Model : public Object {
public:
    Model(const std::vector<std::shared_ptr<Mesh>> & meshes, const std::string & modelName, 
            std::vector<std::shared_ptr<Texture>> texturesDiffuse = {},     
                std::vector<std::shared_ptr<Texture>> texturesSpecular = {},
                    std::vector<std::shared_ptr<Texture>> texturesEmbient = {});

    Model(const std::vector<Mesh> & meshes);

    void ApplyTextureDiffuse(const std::string & textureDiffuseName);
    void ApplyTextureSpecular(const std::string & textureSpecularName);
    void ApplyTextureAmbient(const std::string & textureEmbientName);

    const std::vector<std::shared_ptr<Mesh>> &    getMeshes()  const;
    const std::vector<std::shared_ptr<Texture>> & getTexturesDiffuse() const;
    const std::vector<std::shared_ptr<Texture>> & getTexturesSpecular() const;
    const std::vector<std::shared_ptr<Texture>> & getTexturesEmbient() const;

private:
    std::vector<std::shared_ptr<Mesh>> _meshes;

    std::vector<std::shared_ptr<Texture>> _texturesDiffuse;
    std::vector<std::shared_ptr<Texture>> _texturesSpecular;
    std::vector<std::shared_ptr<Texture>> _texturesEmbient;
};

#endif