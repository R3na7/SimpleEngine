#ifndef WORLD_HPP
#define WORLD_HPP

#include <unordered_map>
#include <memory>

#include "Mesh.hpp"
#include "Model.hpp"
#include "Camera.hpp"


#include "Lighting/PointLight.hpp"
#include "Lighting/SpotLight.hpp"
#include "Lighting/DirectionLight.hpp"

class World {
public:
    //World() = delete;

    World(const std::string & worldName = "", 
            const std::vector<Camera *> & cameras = {}, const std::vector<Mesh *> & meshes = {});

    std::shared_ptr<Mesh>           add(const Mesh & mesh);
    std::shared_ptr<Model>          add(const Model & model);
    std::shared_ptr<Camera>         add(const Camera & camera);
    std::shared_ptr<PointLight>     add(const PointLight & pointLight);
    std::shared_ptr<SpotLight>      add(const SpotLight & spotLight);
    std::shared_ptr<DirectionLight> add(const DirectionLight & directionLight);

    void removeMesh          (const std::string & nameMesh);
    void removeCamera        (const std::string & nameCamera);
    void removePointLight    (const std::string & namePointLight);
    void removeSpotLight     (const std::string & nameSpotLight);
    void removeDirectionLight(const std::string & nameDirectionLight);

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
    std::vector<std::shared_ptr<Model>>          _modelVector;
    std::vector<std::shared_ptr<PointLight>>     _pointLightsVector;
    std::vector<std::shared_ptr<SpotLight>>      _spotLightsVector;
    std::vector<std::shared_ptr<DirectionLight>> _directionLightsVector;

    std::unordered_map<std::string, int> _countDuplicated;
    
    std::shared_ptr<Camera> _currentCamera;
};

#endif