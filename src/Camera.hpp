#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Object.hpp"

#pragma once

class Camera : public Object {
public:
    Camera(const glm::vec3 & position = glm::vec3{0.0f, 0.0f, 0.0f}, 
            float fov = 45, float aspect = 800.0f/600.0f, float zNear = 0.1f, float zFar= 100.0f,
            const std::string & objectName = "Camera");
    

    glm::mat4 getViewMatrix()       const;
    glm::mat4 getProjectionMatrix() const;


private:
    glm::mat4 _projectionMatrix;

    float _fov;
    float _aspect;
    float _zNear;
    float _zFar;
};

#endif