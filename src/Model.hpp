#ifndef MODEL_HPP
#define MODEL_HPP

#include <memory>
#include "Mesh.hpp"

class Model : public Object {
public:
    Model(const std::vector<Mesh> & meshes, const std::string & modelName = "model", 
            const std::vector<std::shared_ptr<Texture>> & texturesDiffuse = {},     
                const std::vector<std::shared_ptr<Texture>> & texturesSpecular = {},
                    const std::vector<std::shared_ptr<Texture>> & texturesEmbient = {});

    void ApplyTextureDiffuse(const std::string & textureDiffuseName);
    void ApplyTextureSpecular(const std::string & textureSpecularName);
    void ApplyTextureAmbient(const std::string & textureEmbientName);

    const std::vector<Mesh> &    getMeshes()  const;
    const std::vector<std::shared_ptr<Texture>> & getTexturesDiffuse() const;
    const std::vector<std::shared_ptr<Texture>> & getTexturesSpecular() const;
    const std::vector<std::shared_ptr<Texture>> & getTexturesEmbient() const;

    static Model getCube(const glm::vec4 & color = {1.0f, 1.0f, 1.0f, 1.0f}, float size = 0.5f, const std::string & objectName = "cube");

private:
    std::vector<Mesh> _meshes;

    std::vector<std::shared_ptr<Texture>> _texturesDiffuse;
    std::vector<std::shared_ptr<Texture>> _texturesSpecular;
    std::vector<std::shared_ptr<Texture>> _texturesEmbient;
};

#endif