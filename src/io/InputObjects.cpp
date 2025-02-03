#include "InputObjects.hpp"

InputObjects::InputObjects(GLFWwindow * window, Keyboard * keyboard, Mouse * mouse)
: _window(window), _keyboard(keyboard), _mouse(mouse) {
    glfwSetWindowUserPointer(_window, this);
}
