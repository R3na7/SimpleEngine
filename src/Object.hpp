#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>

class Object {
public:
    Object(const std::string & objectName, const glm::vec3 & position = glm::vec3{0.0f, 0.0f, 0.0f},
            float pitch = 0.0f, float yaw = -90.0f, glm::mat4x4 model = glm::mat4x4(1.0f));
    Object(const Object & object);
    
    glm::mat4 getModel() const;

    void transfrom(const glm::mat4x4 & matrix);
    void translate(const glm::vec3 & point);
    void translate_to_point(const glm::vec3 & point);
    void rotate(const glm::vec3 &v, float angle);
    void rotateRelativePoint(const glm::vec3 & direction, float angle, const glm::vec3 & axis);
    void rotateLeft(float angle);
    void rotateUp(float angle);
    void rotateLookAt(float angle);
    void scale(const glm::vec3 & s);

    void Pitch(float pitch);
    void Yaw  (float yaw);

    void setname(const std::string & objectName);

    glm::vec3   position() const;
    float       getPitch() const;
    float       getYaw  () const; 
    std::string getName () const;
    std::string getId   () const;
    glm::vec3   lookAt  () const;
    glm::vec3   left    () const;
    glm::vec3   up      () const;
protected:
    std::string _objectName;

    float _pitch = 0.0f;
    float _yaw = -90.0f;
    
    glm::vec3 _position{0.0f, 0.0f, 0.0f};
    glm::mat4x4 _model{1.0f};
};

#endif