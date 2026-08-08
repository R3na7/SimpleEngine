#include "Keyboard.hpp"

#include "InputObjects.hpp"

Keyboard::Keyboard(GLFWwindow * window)
: _window(window) {
    if (_window) {
        glfwSetKeyCallback(_window, keyboardCallback);
    }
}
    
bool Keyboard::isPressed(int key) const {
    if (key < 0 || key > GLFW_KEY_LAST) {
        return false;
    }
    return _keys[key];
}

bool Keyboard::isPressedNow(int key) {
    if (key < 0 || key > GLFW_KEY_LAST) {
        return false;
    }
    bool pressed = _keysPressed[key];
    _keysPressed[key] = false;
    return pressed;
}

bool Keyboard::isReleasedNow(int key) {
    if (key < 0 || key > GLFW_KEY_LAST) {
        return false;
    }
    bool release = _keysReleased[key];
    _keysReleased[key] = false;
    return release;
}

void Keyboard::keyboardCallback(GLFWwindow * window, int key, int scancode, int action, int mods) {

    auto * input = static_cast<InputObjects*>(glfwGetWindowUserPointer(window));
    if (!input || !input->_keyboard) {
        return;
    }
    if (key < 0 || key > GLFW_KEY_LAST) {
        return;
    }
    Keyboard * keyboard = input->_keyboard;

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

GLFWwindow * Keyboard::getLinkedWindow() {
    return _window;
}
