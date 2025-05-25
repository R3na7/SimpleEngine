#include "Mesh.hpp"
#include <algorithm>

Mesh::Mesh(const std::vector<Vertex> & vertices, const std::vector<unsigned int> & indices, 
    const glm::vec4 & color, std::string objectName)
: Object(objectName), _vertices(vertices), _indices(indices), _color(color) {
    vaoInit();
    vboInit();
    eboInit();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
}

Mesh::Mesh(const Mesh & other)
: Object(other), _vertices(other._vertices), _indices(other._indices), 
                _texturesDiffuse(other._texturesDiffuse), _texturesSpecular(other._texturesSpecular), _texturesEmbient(other._texturesEmbient),
                _color(other._color) {
    vaoInit();
    vboInit();
    eboInit();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

Mesh::Mesh(Mesh && other)
: Object(other), _vertices(std::move(other._vertices)), _indices(std::move(other._indices)), 
                _texturesDiffuse(std::move(other._texturesDiffuse)), 
                _texturesSpecular(std::move(other._texturesSpecular)), 
                _texturesEmbient(std::move(other._texturesEmbient)),
                _color(other._color), _VAO(other._VAO), _VBO(other._VBO), _EBO(other._EBO) {
    other._VAO = 0;
    other._VBO = 0;
    other._EBO = 0;
}


void Mesh::bindVertexArray() const {
    glBindVertexArray(_VAO);
}

void Mesh::loadTextureDiffuse(Texture * texture) {
    int cnt;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &cnt);
    if (_texturesDiffuse.size() >= cnt) {
        std::cout << "failed to load texture on " << _objectName << ": the number of available texture blocks has been exceeded\n";
    } else {
        _color = {0.0f, 0.0f, 0.0f, 0.0f};
        _texturesDiffuse.push_back(texture);
    }
}
void Mesh::loadTextureSpecular(Texture * texture) {
    int cnt;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &cnt);
    if (_texturesSpecular.size() >= cnt) {
        std::cout << "failed to load texture on " << _objectName << ": the number of available texture blocks has been exceeded\n";
    } else {
        _color = {0.0f, 0.0f, 0.0f, 0.0f};
        _texturesSpecular.push_back(texture);
    }
}
void Mesh::loadTextureEmbient(Texture * texture) {
    int cnt;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &cnt);
    if (_texturesEmbient.size() >= cnt) {
        std::cout << "failed to load texture on " << _objectName << ": the number of available texture blocks has been exceeded\n";
    } else {
        _color = {0.0f, 0.0f, 0.0f, 0.0f};
        _texturesEmbient.push_back(texture);
    }
}
    
void Mesh::removeTextureDiffuse (Texture * texture) {
    _texturesDiffuse.erase(std::find(_texturesDiffuse.begin(), _texturesDiffuse.end(), texture));
}
void Mesh::removeTextureSpecular(Texture * texture) {
    _texturesSpecular.erase(std::find(_texturesSpecular.begin(), _texturesSpecular.end(), texture));
}
void Mesh::removeTextureEmbient (Texture * texture) {
    _texturesEmbient.erase(std::find(_texturesEmbient.begin(), _texturesEmbient.end(), texture));
}

void Mesh::clearTexturesDiffuse() {
    _texturesDiffuse.clear();
}
void Mesh::clearTexturesSpecular() {
    _texturesSpecular.clear();
}
void Mesh::clearTexturesEmbient() {
    _texturesEmbient.clear();
}
void Mesh::clearTextures() {
    clearTexturesDiffuse();
    clearTexturesSpecular();
    clearTexturesEmbient();
}

void Mesh::setColor(const glm::vec4 color)     { _color = color;         }
void Mesh::setShininess(float shininess)       { _shininess = shininess; }

glm::vec4                      Mesh::getColor()            const {  return _color;            }
float                          Mesh::getShininess()        const {  return _shininess;        }
unsigned int                   Mesh::getIndicesCount()     const {  return _indices.size();   }
unsigned int                   Mesh::getVerticesCount()    const {  return _vertices.size();  }
const std::vector<Texture *> & Mesh::getTexturesDiffuse()  const {  return _texturesDiffuse;  }
const std::vector<Texture *> & Mesh::getTexturesSpecular() const {  return _texturesSpecular; }
const std::vector<Texture *> & Mesh::getTexturesEmbient()  const {  return _texturesEmbient;  }



void Mesh::vaoInit() {
    
    glGenVertexArrays(1, &_VAO);

    glBindVertexArray(_VAO);    
}
void Mesh::vboInit() {
    glGenBuffers(1, &_VBO);

    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * _vertices.size(), _vertices.begin().base(), GL_STATIC_DRAW);
}
void Mesh::eboInit() {
    glGenBuffers(1, &_EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * _indices.size(), _indices.begin().base(), GL_STATIC_DRAW);
}

Mesh Mesh::getCube(const glm::vec4 & color, float size, const std::string & objectName) {
    float HS = size; // Half size of the cube

    std::vector<Vertex> vertices = {
        // Front face (Z+)
        Vertex({-HS, -HS,  HS}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}),
        Vertex({ HS, -HS,  HS}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}),
        Vertex({ HS,  HS,  HS}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}),
        Vertex({-HS,  HS,  HS}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}),

        // Back face (Z-)
        Vertex({-HS, -HS, -HS}, {1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}),
        Vertex({ HS, -HS, -HS}, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}),
        Vertex({ HS,  HS, -HS}, {0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}),
        Vertex({-HS,  HS, -HS}, {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}),

        // Left face (X-)
        Vertex({-HS, -HS, -HS}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}),
        Vertex({-HS, -HS,  HS}, {1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}),
        Vertex({-HS,  HS,  HS}, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}),
        Vertex({-HS,  HS, -HS}, {0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}),

        // Right face (X+)
        Vertex({ HS, -HS,  HS}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}),
        Vertex({ HS, -HS, -HS}, {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}),
        Vertex({ HS,  HS, -HS}, {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}),
        Vertex({ HS,  HS,  HS}, {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}),

        // Top face (Y+)
        Vertex({-HS,  HS,  HS}, {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}),
        Vertex({ HS,  HS,  HS}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}),
        Vertex({ HS,  HS, -HS}, {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}),
        Vertex({-HS,  HS, -HS}, {0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}),

        // Bottom face (Y-)
        Vertex({-HS, -HS, -HS}, {0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}),
        Vertex({ HS, -HS, -HS}, {1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}),
        Vertex({ HS, -HS,  HS}, {1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}),
        Vertex({-HS, -HS,  HS}, {0.0f, 1.0f}, {0.0f, -1.0f, 0.0f})
    };

    std::vector<unsigned int> indices = {
        // Front
        0, 1, 2,  0, 2, 3,
        // Back
        4, 5, 6,  4, 6, 7,
        // Left
        8, 9, 10, 8, 10, 11,
        // Right
        12, 13, 14, 12, 14, 15,
        // Top
        16, 17, 18, 16, 18, 19,
        // Bottom
        20, 21, 22, 20, 22, 23
    };
    return Mesh(vertices, indices, color, objectName);
}

Mesh::~Mesh() {   
    glDeleteVertexArrays(1, &_VAO);
    glDeleteBuffers(1, &_VBO);
    glDeleteBuffers(1, &_EBO);
}