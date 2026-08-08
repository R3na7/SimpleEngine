#include "World.hpp"
#include <algorithm>

World::World(const std::string & worldName, 
                const std::vector<Camera *> & cameras, const std::vector<Mesh *> & meshes)
: _worldName(worldName), _currentCamera(nullptr) {
    for (const auto * camera : cameras) {
        if (camera) add(*camera);
    }
    if (_camerasVector.empty()) {
        add(Camera());
    }

    for (const auto * mesh : meshes) {
        if (mesh) add(*mesh);
    }

    _currentCamera = _camerasVector[0];
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

std::shared_ptr<Object> World::operator[](const std::string & id) {    return at(id); }

std::shared_ptr<Object> World::at(const std::string& name) {
    try {
        return _objectsMap.at(name);
    } catch (const std::out_of_range&) {
        throw std::out_of_range("Object with name '" + name + "' not found in world");
    }
}

std::shared_ptr<const Object> World::at(const std::string& name) const {
    try {
        return _objectsMap.at(name);
    } catch (const std::out_of_range&) {
        throw std::out_of_range("Object with name '" + name + "' not found in world");
    }
}

const std::shared_ptr<Camera> &                      World::getCurrentCamera()   const {    return  _currentCamera;        }
const std::vector<std::shared_ptr<Mesh>> &           World::getMeshes()          const {    return _meshesVector;          }
const std::vector<std::shared_ptr<Model>> &          World::getModels()          const {    return _modelsVector;           }
const std::vector<std::shared_ptr<PointLight>> &     World::getPointLights()     const {    return _pointLightsVector;     }
const std::vector<std::shared_ptr<SpotLight>> &      World::getSpotLights()      const {    return _spotLightsVector;      }

const std::vector<std::shared_ptr<DirectionLight>> & World::getDirectionLights() const {    return _directionLightsVector; }

std::shared_ptr<DirectionLight> World::getDirectionLight(const std::string& dirLightName) {

    auto it = std::find_if(_directionLightsVector.begin(), _directionLightsVector.end(),
        [&dirLightName](const std::shared_ptr<DirectionLight>& directionLight) {
            return directionLight && directionLight->getName() == dirLightName;
        });
    
    if (it != _directionLightsVector.end()) {
        return *it;
    }

    DirectionLight directionLight;
    directionLight.setName(dirLightName);
    return add(directionLight);
}

int World::numberSuchNames(const std::string & name) const {
    int suffix = 0;
    std::string newName = name;

    while (_objectsMap.find(newName) != _objectsMap.end()) {
        suffix++;
        newName = name + "_" + std::to_string(suffix);
    }

    return suffix;
}
