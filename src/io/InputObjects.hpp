#ifndef INPUTOBJECTS_HPP
#define INPUTOBJECTS_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Keyboard.hpp"
#include "Mouse.hpp"

struct InputObjects {
    InputObjects(GLFWwindow * window, Keyboard * keyboard = nullptr, Mouse * mouse = nullptr);

    GLFWwindow * _window;
    Keyboard   * _keyboard;
    Mouse * _mouse;
};

#endif