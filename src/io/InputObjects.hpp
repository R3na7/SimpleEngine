#ifndef INPUTOBJECTS_HPP
#define INPUTOBJECTS_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Keyboard.hpp"
#include "Mouse.hpp"

struct InputObjects {
    InputObjects(Keyboard * keyboard = nullptr, Mouse * mouse = nullptr);
    InputObjects(const InputObjects &) = delete;
    InputObjects(InputObjects &&) = delete;
    InputObjects & operator=(const InputObjects &) = delete;
    InputObjects & operator=(InputObjects &&) = delete;

    ~InputObjects();

    GLFWwindow * _window = nullptr;
    Keyboard   * _keyboard = nullptr;
    Mouse      * _mouse = nullptr;
};

#endif
