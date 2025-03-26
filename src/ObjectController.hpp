#ifndef CAMERACONTROLLER_HPP
#define CAMERACONTROLLER_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Object.hpp"
#include "Utilities/Time.hpp"
#include <memory>

#include "io/InputObjects.hpp"

#include <iostream>

class ObjectController {
public:
    ObjectController(std::shared_ptr<Object> object, InputObjects * controller);

    void handleKeyboardInput();
    void handleMouseInput();
private:
    std::shared_ptr<Object> _object;
    InputObjects * _controller;
};

#endif