#include "Camera.hpp"

Camera::Camera(const glm::vec3 & position, float fov, float aspect, float zNear, float zFar, const std::string & objectName)
: Object(objectName, position), _fov(fov), _aspect(aspect), _zNear(zNear), _zFar(zFar) {
    _projectionMatrix = glm::perspective(fov, aspect, zNear, zFar);
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(_position, _position + lookAt(), glm::vec3{0.0f, 1.0f, 0.0f}); 
}
glm::mat4 Camera::getProjectionMatrix() const {     return _projectionMatrix;   }

