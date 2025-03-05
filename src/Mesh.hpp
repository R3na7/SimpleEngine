#ifndef MESH_HPP
#define MESH_HPP


#include "Object.hpp"
#include "Texture.hpp"


struct Vertex {

    Vertex() = default;

    Vertex(const glm::vec3 & vertex, const glm::vec2 & texture = glm::vec2{0.0f, 0.0f}, glm::vec3 normal = glm::vec3{0.0f, 0.0f, 0.0f}) 
    : x(vertex.x), y(vertex.y), z(vertex.z), 
      xTextureCord(texture.x), yTextureCord(texture.y),
      normalX(normal.x), normalY(normal.y), normalZ(normal.z) {}
      
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    float xTextureCord = 0.0f;
    float yTextureCord = 0.0f;
    
    float normalX = 0.0f;
    float normalY = 0.0f;
    float normalZ = 0.0f;

};

class Mesh : public Object {
public:

    Mesh(const std::vector<Vertex> & vertices, const std::vector<unsigned int> & indices, const glm::vec4 & color = {1.0f, 1.0f, 1.0f, 1.0f}, std::string objectName = "Mesh");

    Mesh(const Mesh & mesh);

    Mesh (Mesh &&) noexcept = default;

    Mesh & operator=(Mesh && mesh) noexcept = default;

    void bindVertexArray() const;

    void loadTextureDiffuse   (Texture * texture);
    void loadTextureSpecular  (Texture * texture);
    void loadTextureEmbient   (Texture * texture);

    void removeTextureDiffuse (Texture * texture);
    void removeTextureSpecular(Texture * texture);
    void removeTextureEmbient (Texture * texture);

    void clearTexturesDiffuse();
    void clearTexturesSpecular();
    void clearTexturesEmbient();
    void clearTextures();

    void setColor(const glm::vec4 color);
    void setShininess(float shininess);

    glm::vec4                      getColor()            const;
    float                          getShininess()        const;
    unsigned int                   getIndicesCount()     const;
    unsigned int                   getVerticesCount()    const;
    const std::vector<Texture *> & getTexturesDiffuse()  const;
    const std::vector<Texture *> & getTexturesSpecular() const;
    const std::vector<Texture *> & getTexturesEmbient()  const;

    static Mesh getCube(const std::string & objectName, const glm::vec4 & color);

    ~Mesh();
private:
    std::vector<Vertex> _vertices{};
    std::vector<unsigned int> _indices{};
    std::vector<Texture *> _texturesDiffuse;
    std::vector<Texture *> _texturesSpecular;
    std::vector<Texture *> _texturesEmbient;

    glm::vec4 _color;
    float _shininess = 64.0f;

    unsigned int _VAO;
    unsigned int _VBO;
    unsigned int _EBO;

    void vaoInit();
    void vboInit();
    void eboInit();
};

#endif