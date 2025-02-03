#include "PointLight.hpp"

PointLight::PointLight(const glm::vec3 & ambient, 
                       const glm::vec3 & diffuse, 
                       const glm::vec3 & specular,
                       const glm::vec3 & position,
                       float constant, float linear, float quadratic) 
: Light(ambient, diffuse, specular), Object("pointLight", position), _constant(constant), _linear(linear), _quadratic(quadratic) {}

float PointLight::getConstant () const {    return _constant;    }
float PointLight::getLinear   () const {    return _linear;      }
float PointLight::getQuadratic() const {    return _quadratic;   }