#include "Object.hpp"
#include <iostream>


Object::Object(const std::string & objectName, const glm::vec3 & position, float pitch, float yaw, glm::mat4x4 model)
: _objectName(objectName), _position(glm::vec3{0.0f, 0.0f, 0.0f}), _pitch(pitch), _yaw(yaw), _model(model) {}

Object::Object(const Object & object) 
: _objectName(object._objectName), _pitch(object._pitch), _yaw(object._yaw), 
    _position(object._position), _model(object._model) {}


void Object::translate(const glm::vec3 & point) {
    _position += point;
}

void Object::translate_to_point(const glm::vec3 & point) {
    _position = point;
}


void Object::rotateRelativePoint(const glm::vec3 & direction, float angle, const glm::vec3 & axis) {

}

void Object::rotate(const glm::vec3 &v, float angle) {
    _model = glm::rotate(_model, angle, v);
}
void Object::rotateLeft(float angle) {
    _model = glm::rotate(_model, glm::radians(angle), glm::vec3{1.0f, 0.0f, 0.0f});
}
void Object::rotateUp(float angle) {
    _model = glm::rotate(_model, glm::radians(angle), glm::vec3{0.0f, 1.0f, 0.0f});
}
void Object::rotateLookAt(float angle) {
    _model = glm::rotate(_model, glm::radians(angle), glm::vec3{0.0f, 0.0f, 1.0f});
}
void Object::scale(const glm::vec3 & s) {
    _model = glm::scale(_model, s);
}

void Object::Pitch(float pitch) { 
    rotateLeft(pitch);  
    _pitch += pitch;   

    if(_pitch >  89.0f)  _pitch =  89.0f;
    if(_pitch < -89.0f)  _pitch = -89.0f;
}   
void Object::Yaw(float yaw) {
    rotateUp(yaw);   
    _yaw += yaw;        
}

void Object::setname(const std::string & objectName) {
    _objectName = objectName;
}

glm::mat4   Object::getModel() const {    return glm::translate(glm::mat4{1.0f}, _position) * _model; }

void Object::transfrom(const glm::mat4x4 & matrix) {
    _model = _model * matrix;
}
glm::vec3   Object::position() const {    return _position;   };
float       Object::getPitch() const {    return _pitch;      };
float       Object::getYaw  () const {    return _yaw;        };
std::string Object::getName () const {    return _objectName; }; 
glm::vec3   Object::lookAt  () const {
    return glm::normalize(glm::vec3{
        std::cos(glm::radians(_yaw  )) * std::cos(glm::radians(_pitch)), 
        std::sin(glm::radians(_pitch)),
        std::sin(glm::radians(_yaw  )) * std::cos(glm::radians(_pitch))
    });
}
glm::vec3   Object::left() const {    return glm::normalize(glm::cross({0.0f, 1.0f, 0.0f}, lookAt()));    }
glm::vec3   Object::up  () const {    return glm::normalize(glm::cross(lookAt(), left()));                }