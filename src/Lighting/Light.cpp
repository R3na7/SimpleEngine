#include "Light.hpp"

Light::Light(const glm::vec3 & ambient, const glm::vec3 & diffuse, const glm::vec3 & specular) 
: _ambient(ambient), _diffuse(diffuse), _specular(specular) {}

void Light::setAmbient(const glm::vec3 & ambient)   {   _ambient = ambient;   }
void Light::setDiffuse(const glm::vec3 & diffuse)   {   _diffuse = diffuse;   }
void Light::setSpecular(const glm::vec3 & specular) {   _specular = specular; }
void Light::setVisible(const bool & _turn)          {   _visible = _turn;     }

glm::vec3 Light::getAmbient () const {  return _ambient;    }
glm::vec3 Light::getDiffuse () const {  return _diffuse;    }
glm::vec3 Light::getSpecular() const {  return _specular;   }
bool      Light::isVisible()   const { return _visible;     }