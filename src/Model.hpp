#ifndef MODEL_HPP
#define MODEL_HPP

#include <memory>
#include "Mesh.hpp"

class Model : public Object {
public:
    Model(const std::vector<Mesh> & meshes, const std::string & modelName = "model", 
            const std::vector<Texture> & texturesDiffuse = {},     
                const std::vector<Texture> & texturesSpecular = {},
                    const std::vector<Texture> & texturesEmbient = {});

    void ApplyTextureDiffuse(const std::string & textureDiffuseName);
    void ApplyTextureSpecular(const std::string & textureSpecularName);
    void ApplyTextureAmbient(const std::string & textureEmbientName);

    const std::vector<Mesh> &    getMeshes()  const;
    const std::vector<Texture> & getTexturesDiffuse() const;
    const std::vector<Texture> & getTexturesSpecular() const;
    const std::vector<Texture> & getTexturesEmbient() const;

    static Model getCube(const glm::vec4 & color = {1.0f, 1.0f, 1.0f, 1.0f}, float size = 0.5f, const std::string & objectName = "cube");

private:
    std::vector<Mesh> _meshes;

    std::vector<Texture> _texturesDiffuse;
    std::vector<Texture> _texturesSpecular;
    std::vector<Texture> _texturesEmbient;
};

#endif