#ifndef WORLD_HPP
#define WORLD_HPP

#include <unordered_map>
#include <memory>
#include <algorithm>
#include <type_traits>

#include "Mesh.hpp"
#include "Model.hpp"
#include "Camera.hpp"


#include "Lighting/PointLight.hpp"
#include "Lighting/SpotLight.hpp"
#include "Lighting/DirectionLight.hpp"

template <typename T>
concept WorldObjectType = 
    std::is_same_v<std::remove_cvref_t<T>, Mesh> ||
    std::is_same_v<std::remove_cvref_t<T>, Model> ||
    std::is_same_v<std::remove_cvref_t<T>, PointLight> ||
    std::is_same_v<std::remove_cvref_t<T>, SpotLight> ||
    std::is_same_v<std::remove_cvref_t<T>, DirectionLight> ||
    std::is_same_v<std::remove_cvref_t<T>, Camera>;

class World {
public:

    World(const std::string & worldName = "", 
            const std::vector<Camera *> & cameras = {}, const std::vector<Mesh *> & meshes = {});

    template <WorldObjectType T>
    std::shared_ptr<std::remove_cvref_t<T>> add(const T& obj);

    template<WorldObjectType T>
    void remove(T & obj);

    template<WorldObjectType T>
    void remove(const std::string& name);

    template<WorldObjectType T>
    void remove(const std::shared_ptr<T> & obj);

    bool saveWorld();
    bool loadWorld();

    void setCurrentCamera(const std::string & cameraName);

    std::shared_ptr<Object>       operator[](const std::string & name);
    std::shared_ptr<Object>       at(const std::string & name);
    std::shared_ptr<const Object> at(const std::string & name) const;

    const std::shared_ptr<Camera> &                      getCurrentCamera()   const;
    const std::vector<std::shared_ptr<Mesh>> &           getMeshes()          const;
    const std::vector<std::shared_ptr<Model>> &          getModels()          const;
    const std::vector<std::shared_ptr<PointLight>> &     getPointLights()     const; 
    const std::vector<std::shared_ptr<SpotLight>> &      getSpotLights()      const;

    const std::vector<std::shared_ptr<DirectionLight>> & getDirectionLights() const;
    std::shared_ptr<DirectionLight> getDirectionLight(const std::string & dirLightName);
private:
    std::string _worldName;

    std::unordered_map<std::string, std::shared_ptr<Object>> _objectsMap;

    std::vector<std::shared_ptr<Camera>>         _camerasVector;
    std::vector<std::shared_ptr<Mesh>>           _meshesVector;
    std::vector<std::shared_ptr<Model>>          _modelsVector;
    std::vector<std::shared_ptr<PointLight>>     _pointLightsVector;
    std::vector<std::shared_ptr<SpotLight>>      _spotLightsVector;
    std::vector<std::shared_ptr<DirectionLight>> _directionLightsVector;
    
    std::shared_ptr<Camera> _currentCamera;

    int numberSuchNames(const std::string & name) const;
};

template <WorldObjectType T>
std::shared_ptr<std::remove_cvref_t<T>> World::add(const T& obj) {
    using ObjectType = std::remove_cvref_t<T>;

    int suffix = 0;
    std::string newName = obj.getName();

    if constexpr (std::is_same_v<ObjectType, DirectionLight>) {
        auto nameExists = [this](const std::string & name) {
            return std::any_of(_directionLightsVector.begin(), _directionLightsVector.end(),
                [&name](const std::shared_ptr<DirectionLight> & light) {
                    return light && light->getName() == name;
                });
        };

        while (nameExists(newName)) {
            ++suffix;
            newName = obj.getName() + "_" + std::to_string(suffix);
        }
    } else {
        suffix = numberSuchNames(obj.getName());
        if (suffix != 0) newName += "_" + std::to_string(suffix);
    }

    auto newObj = std::make_shared<ObjectType>(obj);
    newObj->setName(newName);
    
    if constexpr (std::is_same_v<ObjectType, Mesh>) {
        _meshesVector.push_back(newObj);
        _objectsMap[newName] = newObj;
    } else if constexpr (std::is_same_v<ObjectType, Model>) {
        _modelsVector.push_back(newObj);
        _objectsMap[newName] = newObj;
    } else if constexpr (std::is_same_v<ObjectType, Camera>) {
        _camerasVector.push_back(newObj);
        _objectsMap[newName] = newObj;
    } else if constexpr (std::is_same_v<ObjectType, PointLight>) {
        _pointLightsVector.push_back(newObj);
        _objectsMap[newName] = newObj;
    } else if constexpr (std::is_same_v<ObjectType, SpotLight>) {
        _spotLightsVector.push_back(newObj);
        _objectsMap[newName] = newObj;
    } else if constexpr (std::is_same_v<ObjectType, DirectionLight>) {
        _directionLightsVector.push_back(newObj);
    } else {
        std::cout << "Unsupported type for World::add. Object: " + obj.getName();
        return nullptr;
    }
    
    return newObj;
}

template<WorldObjectType T>
void World::remove(const std::string& name) {
    using ObjectType = std::remove_cvref_t<T>;

    auto& targetVector = [this]() -> auto& {
        if constexpr (std::is_same_v<ObjectType, Mesh>) {
            return _meshesVector;
        } else if constexpr (std::is_same_v<ObjectType, Model>) {
            return _modelsVector;
        } else if constexpr (std::is_same_v<ObjectType, Camera>) {
            return _camerasVector;
        } else if constexpr (std::is_same_v<ObjectType, PointLight>) {
            return _pointLightsVector;
        } else if constexpr (std::is_same_v<ObjectType, SpotLight>) {
            return _spotLightsVector;
        } else if constexpr (std::is_same_v<ObjectType, DirectionLight>) {
            return _directionLightsVector;
        }
    }();

    const auto previousSize = targetVector.size();
    targetVector.erase(
        std::remove_if(targetVector.begin(), targetVector.end(),
            [&name](const std::shared_ptr<ObjectType>& obj) {
                return obj && obj->getName() == name;
            }),
        targetVector.end()
    );

    if (targetVector.size() == previousSize) {
        return;
    }

    if constexpr (!std::is_same_v<ObjectType, DirectionLight>) {
        _objectsMap.erase(name);
    }

    if constexpr (std::is_same_v<ObjectType, Camera>) {
        if (_currentCamera && _currentCamera->getName() == name) {
            if (targetVector.empty()) {
                _currentCamera = add(Camera());
            } else {
                _currentCamera = targetVector.front();
            }
        }
    }
}

template<WorldObjectType T>
void World::remove(T& obj) {
    remove<T>(obj.getName());
}

template<WorldObjectType T>
void World::remove(const std::shared_ptr<T> & obj) {
    if (obj) {
        remove<T>(obj->getName());
    }
}




#endif
