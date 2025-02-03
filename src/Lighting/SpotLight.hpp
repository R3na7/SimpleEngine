#ifndef SPOTLIGHT_HPP
#define SPOTLIGHT_HPP

#include "Light.hpp"
#include "../Object.hpp"

class SpotLight : public Light, public Object {
public:
    SpotLight(const glm::vec3 ambient   = {0.1f, 0.1f, 0.1},
              const glm::vec3 diffuse   = {0.8f, 0.8f, 0.8f},
              const glm::vec3 specular  = {0.8f, 0.8f, 0.8f},
              const glm::vec3 position  = {0.0f, 0.0f, 0.0f},
              const glm::vec3 direction = {1.0f, 0.0f, 0.0f},
              float constant = 1.0f, float linear = 0.09f, float quadratic = 0.032f,
              float cutOff = 12.5f, float outerCutOff = 17.5f);

    void setDirection   (const glm::vec3 & direction);
    void setCutOff      (float cutOff);
    void setOuterCutOff (float outerCutOff);
    void setConstant    (float constant);
    void setLinear      (float linear);
    void setQuadratic   (float quadratic);


    glm::vec3 getDirection()   const;
    float     getCutOff()      const;
    float     getOuterCutOff() const;
    float     getConstant()    const;
    float     getLinear()      const;
    float     getQuadratic()   const;

private:
    glm::vec3 _direction;
    float _cutOff;
    float _outerCutOff;

    float _constant;
    float _linear;
    float _quadratic;
};

#endif