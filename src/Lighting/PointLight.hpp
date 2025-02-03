#ifndef POINTLIGHT_HPP
#define POINTLIGHT_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Object.hpp"
#include "Light.hpp"

class PointLight : public Light, public Object {
public:
    PointLight(const glm::vec3 & ambient  = {0.1f, 0.1f, 0.1f}, 
               const glm::vec3 & diffuse  = {0.8f, 0.8f, 0.8f}, 
               const glm::vec3 & specular = {0.8f, 0.8f, 0.8f},
               const glm::vec3 & position = {0.0f, 0.0f, 0.0f},
               float constant = 1.0f, float linear = 0.09f, float quadratic = 0.032f);

    float getConstant () const;
    float getLinear   () const;
    float getQuadratic() const;

private:
    float _constant;
    float _linear;
    float _quadratic;
};

#endif