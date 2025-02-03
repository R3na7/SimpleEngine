#include "Keyboard.hpp"

#include "InputObjects.hpp"

Keyboard::Keyboard(GLFWwindow * window)
: _window(window) {
    glfwSetKeyCallback(_window, keyboardCallback);
}
    
bool Keyboard::isPressed(int key) const {
    return _keys[key];
}

bool Keyboard::isPressedNow(int key) {
    bool pressed = _keysPressed[key];
    _keysPressed[key] = false;
    return pressed;
}

bool Keyboard::isReleasedNow(int key) {
    bool release = _keysReleased[key];
    _keysReleased[key] = false;
    return release;
}

void Keyboard::keyboardCallback(GLFWwindow * window, int key, int scancode, int action, int mods) {

    Keyboard * keyboard = (static_cast<InputObjects*>(glfwGetWindowUserPointer(window)))->_keyboard;

    if (keyboard) {
        if (action == GLFW_PRESS) {
            if (!keyboard->isPressed(key)) {
                keyboard->_keysPressed[key] = true;
            }
            keyboard->_keys[key] = true;
        } else if (action == GLFW_RELEASE) {
            keyboard->_keys[key] = false;
            keyboard->_keysReleased[key] = true;
        }
    }
}