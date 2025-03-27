#ifndef WORLD_HPP
#define WORLD_HPP

#include <unordered_map>
#include <memory>
#include <algorithm>

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
    std::shared_ptr<T> add(const T& obj);

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
std::shared_ptr<T> World::add(const T& obj) {
    int suffix = numberSuchNames(obj.getName());
    std::string newName = obj.getName();

    if (suffix != 0) newName += "_" + std::to_string(suffix); 

    auto newObj = std::make_shared<T>(obj);
    newObj->setName(newName);
    
    if constexpr (std::is_same_v<T, Mesh>) {
        _meshesVector.push_back(newObj);
        _objectsMap[newName] = newObj;
    } else if constexpr (std::is_same_v<T, Model>) {
        _modelsVector.push_back(newObj);
        _objectsMap[newName] = newObj;
    } else if constexpr (std::is_same_v<T, Camera>) {
        _camerasVector.push_back(newObj);
        _objectsMap[newName] = newObj;
    } else if constexpr (std::is_same_v<T, PointLight>) {
        _pointLightsVector.push_back(newObj);
        _objectsMap[newName] = newObj;
    } else if constexpr (std::is_same_v<T, SpotLight>) {
        _spotLightsVector.push_back(newObj);
        _objectsMap[newName] = newObj;
    } else if constexpr (std::is_same_v<T, DirectionLight>) {
        _directionLightsVector.push_back(newObj);
    } else {
        std::cout << "Unsupported type for World::add. Object: " + obj.getName();
        return nullptr;
    }
    
    return newObj;
}

template<WorldObjectType T>
void World::remove(const std::string& name) {
    auto& targetVector = [this]() -> auto& {
        if constexpr (std::is_same_v<T, Mesh>) {
            return _meshesVector;
        } else if constexpr (std::is_same_v<T, Model>) {
            return _modelsVector;
        } else if constexpr (std::is_same_v<T, Camera>) {
            return _camerasVector;
        } else if constexpr (std::is_same_v<T, PointLight>) {
            return _pointLightsVector;
        } else if constexpr (std::is_same_v<T, SpotLight>) {
            return _spotLightsVector;
        } else if constexpr (std::is_same_v<T, DirectionLight>) {
            return _directionLightsVector;
        }
    }();

    targetVector.erase(
        std::remove_if(targetVector.begin(), targetVector.end(),
            [&name](const std::shared_ptr<T>& obj) {
                return obj->getName() == name;
            }),
        targetVector.end()
    );

    if constexpr (!std::is_same_v<T, DirectionLight>) {
        _objectsMap.erase(name);
    }
}

template<WorldObjectType T>
void World::remove(T& obj) {
    remove<T>(obj.getName());
}

template<WorldObjectType T>
void World::remove(const std::shared_ptr<T> & obj) {
    remove<T>(obj.getName());
}




#endif