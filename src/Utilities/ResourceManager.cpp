#include "ResourceManager.hpp"
#include "../Renderer.hpp"
#include <iomanip> 

ResourceManager * ResourceManager::_instance = nullptr;
std::string ResourceManager::_filename = "";

void ResourceManager::init() {
    delete _instance;
    _instance = new ResourceManager;
}

std::string ResourceManager::getFilename()  {   return _filename;   }

glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4 &from);

void processNode(aiNode * node, const aiScene * scene, std::vector<Mesh> & meshesModel,
                    std::vector<std::shared_ptr<Texture>> & texturesDiffuseModel, 
                        std::vector<std::shared_ptr<Texture>> & texturesSpecularModel,
                            std::vector<std::shared_ptr<Texture>> & texturesEmbientModel, glm::mat4 parentTransform);

Model ResourceManager::loadModel(const std::string & filename) {
    _filename = filename;
    if (!_instance) {
        std::cout << "File upload error filename: the ResourceManager has not been initialized\n";
        _filename = "";
        return {{}, filename + " model failed load"};
    }
    if (_filename.find(".") == std::string::npos)  {
        std::cout << "Error loading model: File extension not specified (" << _filename << ")\n";
        _filename = "";
        return {{}, filename + " model failed load"};
    }

    Assimp::Importer importer;
    const aiScene* scene = nullptr;

    std::string expansion = _filename.substr(_filename.find_last_of('.'));

    if (expansion == ".obj") 
    {
        scene = importer.ReadFile (
            filename, 
            aiProcess_Triangulate 
            | aiProcess_GlobalScale  
            | aiProcess_FlipUVs
            | aiProcess_JoinIdenticalVertices
        );
    }
    else if (expansion == ".fbx" || expansion == ".FBX") 
    {
        scene = importer.ReadFile (
            filename, 
            aiProcess_Triangulate 
            | aiProcess_GlobalScale  
            | aiProcess_PreTransformVertices
            | aiProcess_JoinIdenticalVertices
        );
    } 
    else 
    {
        std::cout << "Error loading model: this file format is not supported (" << expansion << ")\n";
        _filename = "";
        return {{}, _filename + " model failed load"};
    }

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "Error loading model: " << importer.GetErrorString() << '\n';
        return {{}, ""};
    }

    std::vector<Mesh>    meshesModel;
    std::vector<std::shared_ptr<Texture>> texturesDiffuseModel;
    std::vector<std::shared_ptr<Texture>> texturesSpecularModel;
    std::vector<std::shared_ptr<Texture>> texturesEmbientModel;

    std::string path = _filename.substr(0, _filename.find_last_of("/"));

    for (int i = 0; i < scene->mNumMaterials; ++i) {
        aiMaterial * material = scene->mMaterials[i];

        for (int j = 0; j < material->GetTextureCount(aiTextureType_DIFFUSE); ++j) 
        {
            aiString textureFileName;
            material->GetTexture(aiTextureType_DIFFUSE, j, &textureFileName);
            std::string textureFilePath = path + "/" + textureFileName.C_Str();

            std::cout << textureFilePath << '\n';

            texturesDiffuseModel.emplace_back(std::make_shared<Texture>(textureFilePath));
        }

        for (int j = 0; j < material->GetTextureCount(aiTextureType_SPECULAR); ++j) 
        {
            aiString textureFileName;
            material->GetTexture(aiTextureType_SPECULAR, j, &textureFileName);
            std::string textureFilePath = path + "/" + textureFileName.C_Str();

            float shininess;
            material->Get(AI_MATKEY_SHININESS, shininess);

            texturesSpecularModel.emplace_back(std::make_shared<Texture>(textureFilePath, shininess));        }

        for (int j = 0; j < material->GetTextureCount(aiTextureType_AMBIENT); ++j) 
        {
            aiString textureFileName;
            material->GetTexture(aiTextureType_AMBIENT, j, &textureFileName);
            std::string textureFilePath = path + "/" + textureFileName.C_Str();

            texturesEmbientModel.emplace_back(std::make_shared<Texture>(textureFilePath));
        }                
    }

    processNode(scene->mRootNode, scene, meshesModel, texturesDiffuseModel, texturesSpecularModel, texturesEmbientModel, aiMatrix4x4ToGlm(scene->mRootNode->mTransformation));

    std::string sceneName = scene->mName.length > 0 ? scene->mName.C_Str() : "Unnamed Scene";

    _filename = "";
    return {meshesModel, sceneName, texturesDiffuseModel, texturesSpecularModel, texturesEmbientModel};
}





Mesh processMesh(const aiMesh * meshAi) {
    
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    for (int i = 0; i < meshAi->mNumVertices; ++i) {
        float x = meshAi->mVertices[i].x;
        float y = meshAi->mVertices[i].y;
        float z = meshAi->mVertices[i].z;

        float xTextureCord = 0.0f;
        float yTextureCord = 0.0f;

        if (meshAi->mTextureCoords[0]) {
            xTextureCord = meshAi->mTextureCoords[0][i].x;
            yTextureCord = meshAi->mTextureCoords[0][i].y;
        }

        float normalX = 0.0f;
        float normalY = 0.0f;
        float normalZ = 0.0f;

        if (meshAi->mNormals) {
            normalX = meshAi->mNormals[i].x;
            normalY = meshAi->mNormals[i].y;
            normalZ = meshAi->mNormals[i].z;
        }
        vertices.push_back(Vertex({x, y, z}, {xTextureCord, yTextureCord}, {normalX, normalY, normalZ}));
    }

    for (int i = 0; i < meshAi->mNumFaces; ++i) {
        for (int j = 0; j < meshAi->mFaces[i].mNumIndices; ++j) {
            indices.push_back(meshAi->mFaces[i].mIndices[j]);
        }
    }   

    return {vertices, indices, {1.0f, 1.0f, 1.0f, 1.0f}, meshAi->mName.C_Str()};   
}

void proccesMaterial(aiMaterial * material, Mesh & mesh, const std::string & path,
                    std::vector<std::shared_ptr<Texture>> & texturesDiffuseModel, 
                        std::vector<std::shared_ptr<Texture>> & texturesSpecularModel,
                            std::vector<std::shared_ptr<Texture>> & texturesEmbientModel) {

    for (int j = 0; j < material->GetTextureCount(aiTextureType_DIFFUSE); ++j) {
        aiString textureFileName;

        material->GetTexture(aiTextureType_DIFFUSE, j, &textureFileName);
        for (auto & textureDifuse : texturesDiffuseModel) {
            if (*textureDifuse == path + "/" + textureFileName.C_Str()) {
                mesh.loadTextureDiffuse(textureDifuse.get());
            }    
        }

        material->GetTexture(aiTextureType_SPECULAR, j, &textureFileName);
        for (auto & textureSpecular : texturesSpecularModel) {
            if (*textureSpecular == path + "/" + textureFileName.C_Str()) {
                mesh.loadTextureSpecular(textureSpecular.get());
            }
        } 

        material->GetTexture(aiTextureType_AMBIENT, j, &textureFileName);
        for (auto & textureEmbient : texturesEmbientModel) {
            if (*textureEmbient == path + "/" + textureFileName.C_Str()) {
                mesh.loadTextureEmbient(textureEmbient.get());
            }
        }         
    }       
}

glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4 &from) {
    glm::mat4 to;
    to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
    to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
    to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
    to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
    return to;
}

void processNode(aiNode * node, const aiScene * scene, std::vector<Mesh> & meshesModel,
                    std::vector<std::shared_ptr<Texture>> & texturesDiffuseModel, 
                        std::vector<std::shared_ptr<Texture>> & texturesSpecularModel,
                            std::vector<std::shared_ptr<Texture>> & texturesEmbientModel, glm::mat4 parentTransform) {

    glm::mat4x4 nodeTransform = parentTransform * aiMatrix4x4ToGlm(node->mTransformation);
    std::string path = ResourceManager::getFilename().substr(0, ResourceManager::getFilename().find_last_of("/"));

    for (int i = 0; i < node->mNumMeshes; ++i) {

        aiMesh * meshAi = scene->mMeshes[node->mMeshes[i]];

        Mesh mesh = processMesh(meshAi);
        mesh.transfrom(nodeTransform);

        aiMaterial * material = scene->mMaterials[meshAi->mMaterialIndex];
        proccesMaterial(material, mesh, path, texturesDiffuseModel, texturesSpecularModel, texturesEmbientModel);      

        meshesModel.push_back(std::move(mesh));

    }

    for (int i = 0; i < node->mNumChildren; ++i) {
        processNode(node->mChildren[i], scene, meshesModel, 
                        texturesDiffuseModel, texturesSpecularModel, texturesEmbientModel, nodeTransform);
    }
}