#include "DirectionLight.hpp"

DirectionLight::DirectionLight(const glm::vec3 & ambient, const glm::vec3 & diffuse, const glm::vec3 & specular, 
                               const glm::vec3 & direction)
: Light(ambient, diffuse, specular), _direction(direction) {}

void DirectionLight::setName     (const std::string & name)    {    _name = name;           }
void DirectionLight::setDirection(const glm::vec3 & direction) {    _direction = direction; }

std::string DirectionLight::getName()    const {  return _name;       }
glm::vec3 DirectionLight::getDirection() const {  return _direction;  }