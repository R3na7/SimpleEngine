#ifndef DIRECTIONLIGHT_HPP
#define DIRECTIONLIGHT_HPP

#include <string>

#include "Light.hpp"

class DirectionLight : public Light {
public:
    DirectionLight(const glm::vec3 & ambient  = {0.1f, 0.1f, 0.1f}, 
                   const glm::vec3 & diffuse  = {0.6f, 0.6f, 0.6f}, 
                   const glm::vec3 & specular = {0.6f, 0.6f, 0.6f},
                   const glm::vec3 & direction = {0.6f, 0.8f, 0.0f});

    void setDirection(const glm::vec3 & direction);
    void setName     (const std::string & name);
    
    glm::vec3 getDirection() const;
    std::string getName()    const;

private:
    std::string _name;

    glm::vec3 _direction;
};

#endif