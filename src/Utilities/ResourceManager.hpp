#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP

#include <algorithm>


#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "../Model.hpp"

class ResourceManager {
public:
    static void init();

    static Model loadModel(const std::string & filename);

    static std::string getFilename();
private:
    ResourceManager() = default;

    static std::string _filename;

    static ResourceManager * _instance;

};

#endif