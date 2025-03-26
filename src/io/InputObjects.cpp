#include "InputObjects.hpp"
#include <iostream>

InputObjects::InputObjects(Keyboard * keyboard, Mouse * mouse)
: _keyboard(keyboard), _mouse(mouse) {

    if (_keyboard->getLinkedWindow() == _mouse->getLinkedWindow()) {
        _window = _keyboard->getLinkedWindow();
        glfwSetWindowUserPointer(_window, this);
    } else {
        std::cout << "Error: It is not possible to link input devices linked to different windows\n";
    }
}
