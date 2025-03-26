#include "SpotLight.hpp"

SpotLight::SpotLight(const glm::vec3 ambient, const glm::vec3 diffuse, const glm::vec3 specular, const glm::vec3 position, 
                        float constant, float linear, float quadratic,
                        float cutOff, float outerCutOff) 
: Light(ambient, diffuse, specular), Object("SpotLight", position), 
 _cutOff(cutOff), _outerCutOff(outerCutOff), _constant(constant), _linear(linear), _quadratic(quadratic) {}

void SpotLight::setCutOff     (float cutOff)                { _cutOff      = cutOff;        }
void SpotLight::setOuterCutOff(float outerCutOff)           { _outerCutOff = outerCutOff; }
void SpotLight::setConstant   (float constant)              { _constant    = constant;      }
void SpotLight::setLinear     (float linear)                { _linear      = linear;        }
void SpotLight::setQuadratic  (float quadratic)             { _quadratic   = quadratic;     }

float     SpotLight::getCutOff()      const { return _cutOff;      }
float     SpotLight::getOuterCutOff() const { return _outerCutOff; }
float     SpotLight::getConstant()    const { return _constant;    }
float     SpotLight::getLinear()      const { return _linear;      }
float     SpotLight::getQuadratic()   const { return _quadratic;   }