#include "World.hpp"
#include <algorithm>

World::World(const std::string & worldName, 
                const std::vector<Camera *> & cameras, const std::vector<Mesh *> & meshes)
: _worldName(worldName) {

    for (int i = 0; i < meshes.size(); ++i)  addMesh(*meshes[i]);
    for (int i = 0; i < cameras.size(); ++i) addCamera(*cameras[i]);

    _currentCamera = _camerasVector[0];
}

World::World(const std::string & fileName) {
    // coming soon...
}

std::shared_ptr<Mesh> World::addMesh(const Mesh & mesh) {
    std::string name = mesh.getName();
    if (_countDuplicated.find(name) != _countDuplicated.end()) {
        name = mesh.getName() + "_" + std::to_string(_countDuplicated[name]);
    } else {
        _countDuplicated[name] = 1;
    }

    _meshesVector.push_back(std::make_shared<Mesh>(mesh));
    _meshesVector[_meshesVector.size() - 1]->setname(name);

    _objectsMap[name] = _meshesVector[_meshesVector.size() - 1];
    return _meshesVector[_meshesVector.size() - 1];
}

std::shared_ptr<Model> World::addModel(const Model & model) {
    std::string name = model.getName();
    if (_countDuplicated.find(name) != _countDuplicated.end()) {
        name = model.getName() + "_" + std::to_string(_countDuplicated[name]);
    } else {
        _countDuplicated[name] = 1;
    }

    _modelVector.push_back(std::make_shared<Model>(model));
    _modelVector[_modelVector.size() - 1]->setname(name);

    _objectsMap[name] = _modelVector[_modelVector.size() - 1];
    return _modelVector[_modelVector.size() - 1];
}

std::shared_ptr<Camera> World::addCamera(const Camera & camera) {
    std::string name = camera.getName();
    if (_countDuplicated.find(name) != _countDuplicated.end()) {
        name = camera.getName() + "_" + std::to_string(_countDuplicated[name]);
    } else {
        _countDuplicated[name] = 1;
    }

    _camerasVector.push_back(std::make_shared<Camera>(camera));
    _camerasVector[_camerasVector.size() - 1]->setname(name);

    _objectsMap[name] = _camerasVector[_camerasVector.size() - 1];
    return _camerasVector[_camerasVector.size() - 1];
}

std::shared_ptr<PointLight> World::addPointLight(const PointLight & pointLight = PointLight()) {
    std::string name = pointLight.getName();
    if (_countDuplicated.find(name) != _countDuplicated.end()) {
        name = pointLight.getName() + "_" + std::to_string(_countDuplicated[name]);
    } else {
        _countDuplicated[name] = 1;
    }

    _pointLightsVector.push_back(std::make_shared<PointLight>(pointLight));
    _pointLightsVector[_pointLightsVector.size() - 1]->setname(name);

    _objectsMap[name] = _pointLightsVector[_pointLightsVector.size() - 1];
    return _pointLightsVector[_pointLightsVector.size() - 1];
}

std::shared_ptr<SpotLight> World::addSpotLight(const SpotLight & spotLight = SpotLight()) {
    std::string name = spotLight.getName();
    if (_countDuplicated.find(name) != _countDuplicated.end()) {
        name = spotLight.getName() + "_" + std::to_string(_countDuplicated[name]);
    } else {
        _countDuplicated[name] = 1;
    }

    _spotLightsVector.push_back(std::make_shared<SpotLight>(spotLight));
    _spotLightsVector[_spotLightsVector.size() - 1]->setname(name);

    _objectsMap[name] = _spotLightsVector[_spotLightsVector.size() - 1];
    return _spotLightsVector[_spotLightsVector.size() - 1];
}

std::shared_ptr<DirectionLight> World::addDirectionLight(const DirectionLight & directionLight = DirectionLight{}) {
    std::string name = directionLight.getName();
    if (_countDuplicated.find(name) != _countDuplicated.end()) {
        name = directionLight.getName() + "_" + std::to_string(_countDuplicated[name]);
    } else {
        _countDuplicated[name] = 1;
    }

    _directionLightsVector.push_back(std::make_shared<DirectionLight>(directionLight));
    _directionLightsVector[_directionLightsVector.size() - 1]->setName(name);
    return _directionLightsVector[_directionLightsVector.size() - 1];
}


void World::removeMesh(const std::string & nameMesh) {
    if (_countDuplicated[nameMesh] == 0) {
        std::cout << "object" << nameMesh << " does not exist in the world: " << _worldName << '\n';
    } else {
        for (int i = 0; i < _meshesVector.size(); ++i) {
            if (_meshesVector[i]->getName() == nameMesh) {
                _meshesVector.erase(_meshesVector.begin() + i);
                break;
            }
        }

        for (auto & objectName : _objectsMap) {
            if (objectName.first == nameMesh) {
                _objectsMap.erase(nameMesh);
                break;
            }
        }  
    }
}

void World::removeCamera(const std::string & nameCamera) {
    if (_currentCamera->getName() == nameCamera) {
        std::cout << "The camera " << nameCamera << " is currently in use and cannot be deleted.";
    }

    if (_countDuplicated[nameCamera] == 0) {
        std::cout << "object " << nameCamera << " does not exist in the world: " << _worldName << '\n';
    } else {
        for (int i = 0; i < _camerasVector.size(); ++i) {
            if (_camerasVector[i]->getName() == nameCamera) {
                _camerasVector.erase(_camerasVector.begin() + i);
                break;
            }
        }

        for (auto & objectName : _objectsMap) {
            if (objectName.first == nameCamera) {
                _objectsMap.erase(nameCamera);
                break;
            }
        }
    }
}
void World::removePointLight(const std::string & namePointLight) {
    if (_countDuplicated[namePointLight] == 0) {
        std::cout << "object " << namePointLight << " does not exist in the world: " << _worldName << '\n';
    } else {
        for (int i = 0; i < _pointLightsVector.size(); ++i) {
            if (_pointLightsVector[i]->getName() == namePointLight) {
                _pointLightsVector.erase(_pointLightsVector.begin() + i);
                break;
            }
        }

        for (auto & objectName : _objectsMap) {
            if (objectName.first == namePointLight) {
                _objectsMap.erase(namePointLight);
                break;
            }
        }  
    }
}

void World::removeSpotLight(const std::string & nameSpotLight) {
    if (_countDuplicated[nameSpotLight] == 0) {
        std::cout << "object " << nameSpotLight << " does not exist in the world: " << _worldName << '\n';
    } else {
        for (int i = 0; i < _spotLightsVector.size(); ++i) {
            if (_spotLightsVector[i]->getName() == nameSpotLight) {
                _spotLightsVector.erase(_spotLightsVector.begin() + i);
                break;
            }
        }

        for (auto & objectName : _objectsMap) {
            if (objectName.first == nameSpotLight) {
                _objectsMap.erase(nameSpotLight);
                break;
            }
        }  
    }
}

void World::removeDirectionLight(const std::string & nameDirectionLight) {
    if (_countDuplicated[nameDirectionLight] == 0) {
        std::cout << "object " << nameDirectionLight << " does not exist in the world: " << _worldName << '\n';
    } else {
        for (int i = 0; i < _directionLightsVector.size(); ++i) {
            if (_directionLightsVector[i]->getName() == nameDirectionLight) {
                _directionLightsVector.erase(_directionLightsVector.begin() + i);
                break;
            }
        }
    }
}

bool World::saveWorld() {
    // coming soon...
    return false;
}

bool World::loadWorld() {
    // coming soon...
    return false;
}

void World::setCurrentCamera(const std::string & cameraName) {    
    for (int i = 0; i < _camerasVector.size(); ++i) {
        if (_camerasVector[i]->getName() == cameraName) {
            _currentCamera = _camerasVector[i];
            break;
        }
    }   
}

Object * World::operator[](const std::string & id) {    return _objectsMap[id].get(); }

const Camera &                                       World::getCurrentCamera()   const {    return *_currentCamera;        }
const std::vector<std::shared_ptr<Mesh>> &           World::getMeshes()          const {    return _meshesVector;          }
const std::vector<std::shared_ptr<Model>> &          World::getModels()          const {    return _modelVector;           }
const std::vector<std::shared_ptr<PointLight>> &     World::getPointLights()     const {    return _pointLightsVector;     }
const std::vector<std::shared_ptr<SpotLight>> &      World::getSpotLights()      const {    return _spotLightsVector;      }

const std::vector<std::shared_ptr<DirectionLight>> & World::getDirectionLights() const {    return _directionLightsVector; }

std::shared_ptr<DirectionLight> World::getDirectionLight(const std::string& dirLightName) {

    auto it = std::find_if(_directionLightsVector.begin(), _directionLightsVector.end(),
        [&dirLightName](const std::shared_ptr<DirectionLight>& directionLight) {
            return directionLight->getName() == dirLightName;
        });
    
    if (it != _directionLightsVector.end()) {
        return *it;
    }

    return addDirectionLight();
}