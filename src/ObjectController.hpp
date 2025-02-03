#ifndef CAMERACONTROLLER_HPP
#define CAMERACONTROLLER_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Object.hpp"
#include "Utilities/Time.hpp"

#include "io/InputObjects.hpp"

#include <iostream>

class ObjectController {
public:
    ObjectController(Object * object, InputObjects * controller);

    void keyboardInputs();
    void mouseInputs();
private:
    Object * _object;
    InputObjects * _controller;
};

#endif