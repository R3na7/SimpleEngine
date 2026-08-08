#include "InputObjects.hpp"
#include <iostream>

InputObjects::InputObjects(Keyboard * keyboard, Mouse * mouse)
: _window(nullptr), _keyboard(keyboard), _mouse(mouse) {

    if (!_keyboard || !_mouse) {
        std::cout << "Error: InputObjects requires both keyboard and mouse\n";
        _window = nullptr;
        return;
    }
    GLFWwindow * keyboardWindow = _keyboard->getLinkedWindow();
    GLFWwindow * mouseWindow = _mouse->getLinkedWindow();

    if (keyboardWindow && keyboardWindow == mouseWindow) {
        _window = keyboardWindow;
        glfwSetWindowUserPointer(_window, this);
    } else {
        std::cout << "Error: It is not possible to link input devices linked to different windows\n";
    }
}

InputObjects::~InputObjects() {
    if (_window && glfwGetWindowUserPointer(_window) == this) {
        glfwSetWindowUserPointer(_window, nullptr);
    }
}
