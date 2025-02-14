#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Light {
public:
    Light(const glm::vec3 & ambient, const glm::vec3 & diffuse, const glm::vec3 & specular);

    void setAmbient(const glm::vec3 & ambient);
    void setDiffuse(const glm::vec3 & diffuse);
    void setSpecular(const glm::vec3 & specular);
    void setVisible(const bool & _turn);

    glm::vec3 getAmbient () const;
    glm::vec3 getDiffuse () const;
    glm::vec3 getSpecular() const;
    bool      isVisible()   const;

protected:
    glm::vec3 _ambient;
    glm::vec3 _diffuse;
    glm::vec3 _specular;

    bool _visible = true;
};

#endif